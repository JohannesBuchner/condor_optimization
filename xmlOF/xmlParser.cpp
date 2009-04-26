/**
 ****************************************************************************
 * <P> XML.c - implementation file for basic XML parser written in ANSI C++ 
 * for portability.
 * It works by using recursion and a node tree for breaking down the elements
 * of an XML document.  </P>
 *
 * @version     V1.0
 *
 * @author      Frank Vanden Berghen
 * based on original implementation by Martyn C Brown
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * If you want to include this tools in any commercial product, 
 * you can contact the author at fvandenb@iridia.ulb.ac.be
 * 
 ****************************************************************************
 */

#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include "xmlParser.h"

#ifndef WIN32
int _tcslen(const char *c)   { return strlen(c); }
int _tcsnicmp(const char *c1, const char *c2, int l) { return strncasecmp(c1,c2,l); }
char *_tcsstr(const char *c1, const char *c2) { return strstr(c1,c2); }
char *_tcschr(const char *c1, int c2) { return strchr(c1,c2); }
char *_tcscpy(char *c1, const char *c2) { return strcpy(c1,c2); }
#endif

XMLNode XMLNode::emptyXMLNode;
XMLClear XMLNode::emptyXMLClear={ NULL, NULL, NULL};
XMLAttribute XMLNode::emptyXMLAttribute={ NULL, NULL};

// Enumeration used to decipher what type a token is
typedef enum TokenType
{
    eTokenText = 0,
    eTokenQuotedText,
    eTokenTagStart,         /* "<"            */
    eTokenTagEnd,           /* "</"           */
    eTokenCloseTag,         /* ">"            */
    eTokenEquals,           /* "="            */
    eTokenDeclaration,      /* "<?"           */
    eTokenShortHandClose,   /* "/>"           */
    eTokenClear,
    eTokenError
} TokenType;

#define INDENTCHAR    _T('\t')

typedef struct ClearTag
{
    LPTSTR lpszOpen;
    LPTSTR lpszClose;
} ClearTag;

// Main structure used for parsing XML
typedef struct XML
{
    LPTSTR                 lpXML;
    int                    nIndex;
    enum XMLError          error;
    LPCTSTR                lpEndTag;
    int                    cbEndTag;
    LPTSTR                lpNewElement;
    int                    cbNewElement;
    int                    nFirst;
    ClearTag               *pClrTags;
} XML;

typedef struct 
{
    ClearTag    *pClr;
    LPTSTR     pStr;
} NextToken;

// Enumeration used when parsing attributes
typedef enum Attrib
{
    eAttribName = 0,
    eAttribEquals,
    eAttribValue
} Attrib;

// Enumeration used when parsing elements to dictate whether we are currently
// inside a tag
typedef enum Status
{
    eInsideTag = 0,
    eOutsideTag
} Status;

// update "order" information when deleting a content of a XMLNode
void XMLNode::removeOrderElement(XMLNodeData *d, XMLElementType t, int index)
{
    int j=(int)((index<<2)+t),i=0,n=nElement(d)+1, *o=d->pOrder;
    while ((o[i]!=j)&&(i<n)) i++;
    n--;
    memmove(o+i, o+i+1, (n-i)*sizeof(int));
    for (;i<n;i++)
        if ((o[i]&3)==(int)t) o[i]-=4;
}

// Obtain the next character from the string. 
static inline TCHAR getNextChar(XML *pXML)
{
	TCHAR ch = pXML->lpXML[pXML->nIndex];	
	if (ch!=0) pXML->nIndex++;
	return ch;
}

// Find next non-white space character.  
static TCHAR FindNonWhiteSpace(XML *pXML)
{
    TCHAR ch;
    int nExit = FALSE;

    assert(pXML);

    // Iterate through characters in the string until we find a NULL or a
    // non-white space character
    while((nExit == FALSE) && (ch = getNextChar(pXML)))
    {
        switch(ch)
        {
        // Ignore white space
        case _T('\n'):
        case _T(' '):
        case _T('\t'):
        case _T('\r'): continue;            
        default: nExit = TRUE;
        }
    }
    return ch;
}

// Find the next token in a string.
// pcbToken contains the number of characters which have been read.
static NextToken GetNextToken(XML *pXML, int *pcbToken, TokenType *pType)
{
    NextToken        result;
    LPTSTR          lpXML;
    TCHAR            ch;
    TCHAR            chTemp;
    int              nSize;
    int              nFoundMatch;
    int              nExit;
    int              n;
    LPCTSTR          lpszOpen;
    int              cbOpen;
    int              nIsText = FALSE;

    // Find next non-white space character
    ch = FindNonWhiteSpace(pXML);

    if (ch)
    {
        // Cache the current string pointer
        lpXML = pXML->lpXML;
        result.pStr = &lpXML[pXML->nIndex-1];

        // First check whether the token is in the clear tag list (meaning it 
        // does not need formatting).
        n = 0;

        while(TRUE)
        {
            // Obtain the name of the open part of the clear tag
            lpszOpen = pXML->pClrTags[n].lpszOpen;

            if (lpszOpen)
            {
                // Compare the open tag with the current token
                cbOpen = (int)_tcslen(lpszOpen);
                if (_tcsnicmp(lpszOpen, result.pStr, cbOpen) == 0)
                {
                    result.pClr = &pXML->pClrTags[n];
                    pXML->nIndex += cbOpen-1;
                    *pType  = eTokenClear;
                    return result;
                }
                n++;
            }
            else break;
        }
        // If we didn't find a clear tag then check for standard tokens
        chTemp = 0;
        lpXML = pXML->lpXML;
        
        switch(ch)
        {
        // Check for quotes
        case _T('\''):
        case _T('\"'):
            // Type of token
            *pType = eTokenQuotedText;
            chTemp = ch;

            // Set the size
            nSize = 1;
            nFoundMatch = FALSE;

            // Search through the string to find a matching quote
            while((ch = getNextChar(pXML)))
            {
                nSize++;
                if (ch == chTemp)
                {
                    nFoundMatch = TRUE;
                    break;
                }
            }

            // If we failed to find a matching quote
            if (nFoundMatch == FALSE)
            {
                // Indicate error
                pXML->error = eXMLErrorNoMatchingQuote;                
                *pType = eTokenError;
            }

            //  4.02.2002 
            if (FindNonWhiteSpace(pXML))
            {
                pXML->nIndex--;
            }

            break;

        // Equals (used with attribute values)
        case _T('='):
            nSize = 1;
            *pType = eTokenEquals;
            break;

        // Close tag
        case _T('>'):
            nSize = 1;
            *pType = eTokenCloseTag;
            break;

        // Check for tag start and tag end
        case _T('<'):

            // Peek at the next character to see if we have an end tag '</',
            // or an xml declaration '<?'
            chTemp = pXML->lpXML[pXML->nIndex];

            // If we have a tag end...
            if (chTemp == _T('/'))
            {
                // Set the type and ensure we point at the next character 
                getNextChar(pXML);
                *pType = eTokenTagEnd;
                nSize = 2;
            }

            // If we have an XML declaration tag
            else if (chTemp == _T('?'))
            {

                // Set the type and ensure we point at the next character 
                getNextChar(pXML);
                *pType = eTokenDeclaration;
                nSize = 2;
            }

            // Otherwise we must have a start tag
            else 
            {
                *pType = eTokenTagStart;
                nSize = 1;
            }
            break;

        // Check to see if we have a short hand type end tag ('/>').
        case _T('/'):

            // Peek at the next character to see if we have a short end tag '/>'
            chTemp = pXML->lpXML[pXML->nIndex];

            // If we have a short hand end tag...
            if (chTemp == _T('>'))
            {
                // Set the type and ensure we point at the next character 
                getNextChar(pXML);
                *pType = eTokenShortHandClose;
                nSize = 2;
                break;
            }

            // If we haven't found a short hand closing tag then drop into the
            // text process

        // Other characters
        default:
            nIsText = TRUE;
        }

        // If this is a TEXT node
        if (nIsText)
        {
            // Indicate we are dealing with text
            *pType = eTokenText;
            nSize = 1;
            nExit = FALSE;

            while((nExit == FALSE) && (ch = getNextChar(pXML)))
            {
                switch(ch)
                {
                // Break when we find white space
                case _T('\n'):
                case _T(' '):
                case _T('\t'):
                case _T('\r'):
                    nExit = TRUE;
                    break;

                // If we find a slash then this maybe text or a short hand end tag.
                case _T('/'):
                    
                    // Peek at the next character to see it we have short hand end tag
                    chTemp = pXML->lpXML[pXML->nIndex];

                    // If we found a short hand end tag then we need to exit the loop
                    if (chTemp == _T('>'))
                    {
                        pXML->nIndex--; //  03.02.2002 
                        nExit = TRUE;
                    }
                    else
                    {
                        nSize++;
                    }
                    break;

                // Break when we find a terminator and decrement the index and
                // column count so that we are pointing at the right character
                // the next time we are called.
                case _T('<'):
                case _T('>'):
                case _T('='):
                    pXML->nIndex--;                    
                    nExit = TRUE;
                    break;

                case 0:
                    nExit = TRUE;
                    break;

                default:
                    nSize++;
                }
            }
        }
        *pcbToken = nSize;
    } else 
    {
        // If we failed to obtain a valid character
        *pcbToken = 0;
        *pType = eTokenError;
        result.pStr=NULL;
    }

    return result;
}

// Parse XML errors into a user friendly string.  
LPCTSTR XMLNode::getError(XMLError error)
{
    LPCTSTR lpszErr = _T("Unknown");
    int n;
    // Structure for errors array
    typedef struct ErrorList
    {
        enum XMLError err;
        LPCTSTR lpszErr;
    } ErrorList;

    // Static array containing helpful error text.
    static struct ErrorList errs[] = 
    {
        { eXMLErrorNone,                _T("No error")                  },
        { eXMLErrorEmpty,               _T("No XML data")               },
        { eXMLErrorFirstNotStartTag,    _T("First token not start tag") },
        { eXMLErrorMissingTagName,      _T("Missing start tag name")    },
        { eXMLErrorMissingEndTagName,   _T("Missing end tag name")      },
        { eXMLErrorNoMatchingQuote,     _T("Unmatched quote")           },
        { eXMLErrorUnmatchedEndTag,     _T("Unmatched end tag")         },
        { eXMLErrorUnexpectedToken,     _T("Unexpected token found")    },
        { eXMLErrorInvalidTag,          _T("Invalid tag found")         },
        { eXMLErrorNoElements,          _T("No elements found")         },
        { eXMLErrorFileNotFound,        _T("File not found")            },
        { eXMLErrorNone,                NULL                            }
    };

    // Iterate through the list of errors to find a matching error
    for(n = 0; errs[n].lpszErr; n++)
    {
        if (errs[n].err == error)
        {
            lpszErr = errs[n].lpszErr;
            break;
        }
    }
    return lpszErr;
}

XMLNode::XMLNode(XMLNode *pParent, LPTSTR lpszName, int isDeclaration)
{
    d=(XMLNodeData*)malloc(sizeof(XMLNodeData));
    d->ref_count=1;

    d->lpszName=lpszName;

    d->nChild= 0;
    d->nText = 0;
    d->nClear = 0;
    d->nAttribute = 0;

    d->isDeclaration = isDeclaration;

    d->pParent = pParent;
    d->pChild= NULL;
    d->pText= NULL;
    d->pClear= NULL;
    d->pAttribute= NULL;
    d->pOrder= NULL;
}

// Add a child node to the given element.
XMLNode XMLNode::AddChild(LPTSTR lpszName, int isDeclaration)
{
    int nc=d->nChild;
    d->pChild=(XMLNode*)realloc(d->pChild,(nc+1)*sizeof(XMLNode));
    d->pChild[nc].d=NULL;
    d->pChild[nc]=XMLNode(this,lpszName,isDeclaration);
    int n=nElement();
    d->pOrder=(int*)realloc(d->pOrder,(n+1)*sizeof(int));
    d->pOrder[n]=(nc<<2)+eNodeChild;
    d->nChild++;
    return d->pChild[nc];
}

// Add an attribute to an element. 
XMLAttribute *XMLNode::AddAttribute(LPTSTR lpszName, LPTSTR lpszValuev)
{
    int na=d->nAttribute;
    d->pAttribute=(XMLAttribute*)realloc(d->pAttribute,(na+1)*sizeof(XMLAttribute));
    XMLAttribute *pAttr=d->pAttribute+na;
    pAttr->lpszName = lpszName;
    pAttr->lpszValue = lpszValuev;
    int n=nElement();
    d->pOrder=(int*)realloc(d->pOrder,(n+1)*sizeof(int));
    d->pOrder[n]=(na<<2)+eNodeAttribute;
    d->nAttribute++;
    return pAttr;
}

// Add text to the element.  
LPTSTR XMLNode::AddText(LPTSTR lpszValue)
{
    int nt=d->nText;
    d->pText=(LPTSTR*)realloc(d->pText,(nt+1)*sizeof(LPTSTR));
    d->pText[nt]=lpszValue;
    int n=nElement();
    d->pOrder=(int*)realloc(d->pOrder,(n+1)*sizeof(int));
    d->pOrder[n]=(nt<<2)+eNodeText;
    d->nText++;
    return d->pText[nt];
}

// Add clear (unformatted) text to the element.
XMLClear *XMLNode::AddClear(LPTSTR lpszValue, LPTSTR lpszOpen, LPTSTR lpszClose)
{
    int nc=d->nClear;
    d->pClear=(XMLClear *)realloc(d->pClear,(nc+1)*sizeof(XMLClear));
    XMLClear *pNewClear=d->pClear+nc;
    pNewClear->lpszValue = lpszValue;
    pNewClear->lpszOpenTag = lpszOpen;
    pNewClear->lpszCloseTag = lpszClose;
    int n=nElement();
    d->pOrder=(int*)realloc(d->pOrder,(n+1)*sizeof(int));
    d->pOrder[n]=(nc<<2)+eNodeClear;
    d->nClear++;
    return pNewClear;
}

// Trim the end of the text to remove white space characters.
static void FindEndOfText(LPCTSTR lpszToken, int *pcbText)
{
    TCHAR   ch;
    int     cbText;
    assert(lpszToken);
    assert(pcbText);
    cbText = (*pcbText)-1;    
    while(TRUE)
    {
        assert(cbText >= 0);
        ch = lpszToken[cbText];
        switch(ch)
        {
        case _T('\r'):
        case _T('\n'):
        case _T('\t'):
        case _T(' '): cbText--; break;
        default: *pcbText = cbText+1; return;
        }
    }
}

// Duplicate a given string. 
static LPTSTR stringDup(LPCTSTR lpszData, int cbData)
{
    if (lpszData==NULL) return NULL;

	LPTSTR lpszNew;
    if (cbData==0) cbData=(int)_tcslen(lpszData);
	lpszNew = (LPTSTR)malloc((cbData+1) * sizeof(TCHAR));
	if (lpszNew)
	{
		memcpy(lpszNew, lpszData, (cbData) * sizeof(TCHAR));
		lpszNew[cbData] = (TCHAR)NULL;
	}
	return lpszNew;
}

// Parse a clear (unformatted) type node.
int XMLNode::ParseClearTag(void *px, void *pa)
{
    XML *pXML=(XML *)px;
    ClearTag *pClear=(ClearTag *)pa;
    int cbTemp = 0;
    LPTSTR lpszTemp;
    LPTSTR lpszXML = &pXML->lpXML[pXML->nIndex];

    // Find the closing tag
    lpszTemp = _tcsstr(lpszXML, pClear->lpszClose);

    // Iterate through the tokens until we find the closing tag.
    if (lpszTemp)
    {
        // Cache the size and increment the index
        cbTemp = (int)(lpszTemp - lpszXML);

        pXML->nIndex += cbTemp;
        pXML->nIndex += (int)_tcslen(pClear->lpszClose);
        
        // Add the clear node to the current element
        AddClear(stringDup(lpszXML,cbTemp), pClear->lpszOpen, pClear->lpszClose);
        return TRUE;
    }

    // If we failed to find the end tag
    pXML->error = eXMLErrorUnmatchedEndTag;
    return FALSE;
}

// Recursively parse an XML element.
int XMLNode::ParseXMLElement(void *pa)
{
    XML *pXML=(XML *)pa;
    int cbToken;
    enum TokenType type;
    NextToken token;
    LPTSTR lpszTemp;
    int cbTemp;
    int nDeclaration;
    LPTSTR lpszText = NULL;
    XMLNode pNew;
    enum Status status; // inside or outside a tag
    enum Attrib attrib = eAttribName;

    assert(pXML);

    // If this is the first call to the function
    if (pXML->nFirst)
    {
        // Assume we are outside of a tag definition
        pXML->nFirst = FALSE;
        status = eOutsideTag;
    } else 
    {
        // If this is not the first call then we should only be called when inside a tag.
        status = eInsideTag;
    }

    // Iterate through the tokens in the document
    while(TRUE)
    {
        // Obtain the next token
        token = GetNextToken(pXML, &cbToken, &type);

        if (type != eTokenError)
        {
            // Check the current status
            switch(status)
            {

            // If we are outside of a tag definition
            case eOutsideTag:

                // Check what type of token we obtained
                switch(type)
                {
                // If we have found text or quoted text
                case eTokenText:
                case eTokenQuotedText:
                case eTokenEquals:
                    if (!lpszText)
                    {
                        lpszText = token.pStr;
                    }                    

                    break;

                // If we found a start tag '<' and declarations '<?'
                case eTokenTagStart:
                case eTokenDeclaration:

                    // Cache whether this new element is a declaration or not
                    nDeclaration = type == eTokenDeclaration;

                    // If we have node text then add this to the element
                    if (lpszText)
                    {
                        cbTemp = (int)(token.pStr - lpszText);
                        FindEndOfText(lpszText, &cbTemp);
                        AddText(stringDup(lpszText,cbTemp));
                        lpszText = NULL;
                    }

                    // Find the name of the tag
                    token = GetNextToken(pXML, &cbToken, &type);

                    // Return an error if we couldn't obtain the next token or
                    // it wasnt text
                    if (type != eTokenText)
                    {
                        pXML->error = eXMLErrorMissingTagName;
                        return FALSE;
                    }

                    // If we found a new element which is the same as this 
                    // element then we need to pass this back to the caller..

                    if (d->lpszName && 
                        _tcsnicmp(d->lpszName, token.pStr, 
                        _tcslen(d->lpszName)) == 0)
                    {
                        // Indicate to the caller that it needs to create a 
                        // new element.
                        pXML->lpNewElement = token.pStr;
                        pXML->cbNewElement = cbToken;
                        return TRUE;
                    } else
                    {
                        // If the name of the new element differs from the name of
                        // the current element we need to add the new element to 
                        // the current one and recurse
                        pNew = AddChild(stringDup(token.pStr,cbToken), nDeclaration);

                        while (!pNew.isEmpty())
                        {
                            // Callself to process the new node.  If we return 
                            // FALSE this means we dont have any more 
                            // processing to do...

                            if (!pNew.ParseXMLElement(pXML))
                            {
                                return FALSE;
                            }
                            else
                            {
                                // If the call to recurse this function 
                                // evented in a end tag specified in XML then 
                                // we need to unwind the calls to this 
                                // function until we find the appropriate node 
                                // (the element name and end tag name must 
                                // match)
                                if (pXML->cbEndTag)
                                {
                                    // If we are back at the root node then we 
                                    // have an unmatched end tag
                                    if (!d->lpszName)
                                    {
                                        pXML->error=eXMLErrorUnmatchedEndTag;
                                        return FALSE;
                                    }

                                    // If the end tag matches the name of this 
                                    // element then we only need to unwind 
                                    // once more...

                                    if (_tcsnicmp(pXML->lpEndTag, 
                                        d->lpszName, 
                                        _tcslen(d->lpszName)) == 0)
                                    {                                    
                                        pXML->cbEndTag = 0;
                                    }

                                    return TRUE;
                                } else 
                                    if (pXML->cbNewElement)
                                    {
                                        // If the call indicated a new element is to 
                                        // be created on THIS element.

                                        // If the name of this element matches the
                                        // name of the element we need to create 
                                        // then we need to return to the caller 
                                        // and let it process the element.

                                        if (_tcsnicmp(pXML->lpNewElement, 
                                            d->lpszName, 
                                            _tcslen(d->lpszName)) == 0)
                                        {                                    
                                            return TRUE;
                                        }

                                        // Add the new element and recurse
                                        pNew = AddChild(stringDup(pXML->lpNewElement,pXML->cbNewElement), FALSE);
                                        pXML->cbNewElement = 0;
                                    }
                                    else 
                                    {
                                        // If we didn't have a new element to create
                                        pNew = emptyXMLNode;

                                    }
                            }
                        }
                    }
                    break;

                // If we found an end tag
                case eTokenTagEnd:

                    // If we have node text then add this to the element
                    if (lpszText)
                    {
                        cbTemp = (int)(token.pStr - lpszText);
                        FindEndOfText(lpszText, &cbTemp);
                        AddText(stringDup(lpszText,cbTemp));
                        lpszText = NULL;
                    }
                    
                    // Find the name of the end tag
                    token = GetNextToken(pXML, &cbTemp, &type);                    

                    // The end tag should be text
                    if (type != eTokenText)
                    {
                        pXML->error = eXMLErrorMissingEndTagName;
                        return FALSE;
                    }
                    lpszTemp = token.pStr;

                    // After the end tag we should find a closing tag
                    token = GetNextToken(pXML, &cbToken, &type);
                    if (type != eTokenCloseTag)
                    {
                        pXML->error = eXMLErrorMissingEndTagName;
                        return FALSE;
                    }

                    // We need to return to the previous caller.  If the name 
                    // of the tag cannot be found we need to keep returning to
                    // caller until we find a match
                    if (_tcsnicmp(lpszTemp, d->lpszName, 
                        _tcslen(d->lpszName)) != 0)
                    {
                        pXML->lpEndTag = lpszTemp;
                        pXML->cbEndTag = cbTemp;
                    }

                    // Return to the caller
                    return TRUE;

                // If we found a clear (unformatted) token
                case eTokenClear:
                    // If we have node text then add this to the element
                    if (lpszText)
                    {
                        cbTemp = (int)(token.pStr - lpszText);
                        FindEndOfText(lpszText, &cbTemp);
                        AddText(stringDup(lpszText,cbTemp));
                        lpszText = NULL;
                    }

                    if (!ParseClearTag(pXML, token.pClr))
                    {
                        return FALSE;
                    }
                    break;

                // Errors...
                case eTokenCloseTag:          /* '>'         */
                case eTokenShortHandClose:    /* '/>'        */
                    pXML->error = eXMLErrorUnexpectedToken;
                    return FALSE;
                default:
                    break;
                }
                break;

            // If we are inside a tag definition we need to search for attributes
            case eInsideTag:

                // Check what part of the attribute (name, equals, value) we 
                // are looking for.
                switch(attrib) 
                {
                // If we are looking for a new attribute
                case eAttribName:

                    // Check what the current token type is
                    switch(type)
                    {
                    // If the current type is text...
                    // Eg.  'attribute'
                    case eTokenText:
                        // Cache the token then indicate that we are next to 
                        // look for the equals
                        lpszTemp = token.pStr;
                        cbTemp = cbToken;
                        attrib = eAttribEquals;
                        break;

                    // If we found a closing tag...
                    // Eg.  '>'
                    case eTokenCloseTag:
                        // We are now outside the tag
                        status = eOutsideTag;
                        break;

                    // If we found a short hand '/>' closing tag then we can 
                    // return to the caller
                    case eTokenShortHandClose:
                        return TRUE;

                    // Errors...
                    case eTokenQuotedText:    /* '"SomeText"'   */
                    case eTokenTagStart:      /* '<'            */
                    case eTokenTagEnd:        /* '</'           */
                    case eTokenEquals:        /* '='            */
                    case eTokenDeclaration:   /* '<?'           */
                    case eTokenClear:
                        pXML->error = eXMLErrorUnexpectedToken;
                        return FALSE;
                    default: break;
                    }
                    break;

                // If we are looking for an equals
                case eAttribEquals:
                    // Check what the current token type is
                    switch(type)
                    {
                    // If the current type is text...
                    // Eg.  'Attribute AnotherAttribute'
                    case eTokenText:
                        // Add the unvalued attribute to the list
                        AddAttribute(stringDup(lpszTemp,cbTemp), NULL);
                        // Cache the token then indicate.  We are next to 
                        // look for the equals attribute
                        lpszTemp = token.pStr;
                        cbTemp = cbToken;
                        break;

                    // If we found a closing tag 'Attribute >' or a short hand 
                    // closing tag 'Attribute />'
                    case eTokenShortHandClose:
                    case eTokenCloseTag:
                        // If we are a declaration element '<?' then we need
                        // to remove extra closing '?' if it exists
                        if (d->isDeclaration && 
                            (lpszTemp[cbTemp-1]) == _T('?'))
                        {
                            cbTemp--;
                        }

                        if (cbTemp)
                        {
                            // Add the unvalued attribute to the list
                            AddAttribute(stringDup(lpszTemp,cbTemp), NULL);
                        }

                        // If this is the end of the tag then return to the caller
                        if (type == eTokenShortHandClose)
                        {
                            return TRUE;
                        }

                        // We are now outside the tag
                        status = eOutsideTag;
                        break;

                    // If we found the equals token...
                    // Eg.  'Attribute ='
                    case eTokenEquals:
                        // Indicate that we next need to search for the value
                        // for the attribute
                        attrib = eAttribValue;
                        break;

                    // Errors...
                    case eTokenQuotedText:    /* 'Attribute "InvalidAttr"'*/
                    case eTokenTagStart:      /* 'Attribute <'            */
                    case eTokenTagEnd:        /* 'Attribute </'           */
                    case eTokenDeclaration:   /* 'Attribute <?'           */
                    case eTokenClear:
                        pXML->error = eXMLErrorUnexpectedToken;
                        return FALSE;
                    default: break;
                    }
                    break;

                // If we are looking for an attribute value
                case eAttribValue:
                    // Check what the current token type is
                    switch(type)
                    {
                    // If the current type is text or quoted text...
                    // Eg.  'Attribute = "Value"' or 'Attribute = Value' or
                    // 'Attribute = 'Value''.
                    case eTokenText:
                    case eTokenQuotedText:
                        // If we are a declaration element '<?' then we need
                        // to remove extra closing '?' if it exists
                        if (d->isDeclaration && 
                            (token.pStr[cbToken-1]) == _T('?'))
                        {
                            cbToken--;
                        }

                        if (cbTemp)
                        {
                            // Add the valued attribute to the list
                            AddAttribute(stringDup(lpszTemp,cbTemp),stringDup(token.pStr,cbToken));
                        }

                        // Indicate we are searching for a new attribute
                        attrib = eAttribName;
                        break;

                    // Errors...
                    case eTokenTagStart:        /* 'Attr = <'          */
                    case eTokenTagEnd:          /* 'Attr = </'         */
                    case eTokenCloseTag:        /* 'Attr = >'          */
                    case eTokenShortHandClose:  /* "Attr = />"         */
                    case eTokenEquals:          /* 'Attr = ='          */
                    case eTokenDeclaration:     /* 'Attr = <?'         */
                    case eTokenClear:
                        pXML->error = eXMLErrorUnexpectedToken;
                        return FALSE;
                        break;
                    default: break;
                    }
                }
            }
        }
        // If we failed to obtain the next token
        else 
        {
            return FALSE;    
        }
    }
}

// Count the number of lines and columns in an XML string.
static void CountLinesAndColumns(LPCTSTR lpXML, int nUpto, XMLResults *pResults)
{
    TCHAR ch;
    int n;

    assert(lpXML);    
    assert(pResults);

    pResults->nLine = 1;
    pResults->nColumn = 1;
    for(n=0; n<nUpto; n++)
    {
        ch = lpXML[n];
        assert(ch);
        if (ch == _T('\n'))
        {
            pResults->nLine++;
            pResults->nColumn = 1;
        }
        else pResults->nColumn++;
    }
}

// Parse XML and return the root element.
XMLNode XMLNode::parseString(LPTSTR lpszXML, XMLResults *pResults)
{
    static struct ClearTag tags[] =
    {
        {    _T("<![CDATA["),    _T("]]>")       },
        {    _T("<PRE>"),        _T("</PRE>")    },
        {    _T("<Script>"),     _T("</Script>") },
        {    _T("<!--"),         _T("-->")       },
        {    _T("<!DOCTYPE"),    _T(">")         },
        {    NULL,               NULL            }
    };
    enum XMLError error;
    XMLNode pElement(NULL,NULL,FALSE);
    struct XML xml ={ NULL, 0, eXMLErrorNone, NULL, 0, NULL, 0, TRUE , NULL};

    xml.lpXML = lpszXML;
    xml.pClrTags = tags;

    // Create header element
    pElement.ParseXMLElement(&xml);
    error = xml.error;

    // If an error occurred
    if (error != eXMLErrorNone)
    {
        // Cleanup: this will destroy all the nodes
        pElement = emptyXMLNode;
    }

    // If we have been given somewhere to place results
    if (pResults)
    {
        pResults->error = error;

        // If we have an error
        if (error != eXMLErrorNone)
        {
            // Find which line and column it starts on.
            CountLinesAndColumns(xml.lpXML, xml.nIndex, pResults);
        }
    }
    return pElement;
}

XMLNode XMLNode::parseFile(char *filename, XMLResults *pResults)
{
    FILE *f=fopen(filename,"rb");
    if (f==NULL)
    {
        if (pResults)
        {
            pResults->error=eXMLErrorFileNotFound;
            pResults->nLine=0;
            pResults->nColumn=0;
        }
        return emptyXMLNode;
    }
    fseek(f,0,SEEK_END);
    int l=ftell(f);
    fseek(f,0,SEEK_SET);
    LPTSTR buf=(char*)malloc(l+1);
    fread(buf,l,1,f);
    fclose(f);
    buf[l]=0;
    XMLNode x=parseString(buf,pResults);
    free(buf);
    return x;
}

XMLNodeContents XMLNode::enumContents(int i)
{ 
    XMLNodeContents c;
    c.type=(XMLElementType)(d->pOrder[i]&3);
    i=(d->pOrder[i])>>2;
    switch (c.type)
    {
        case eNodeChild:     c.child = d->pChild[i];      break;
        case eNodeAttribute: c.attrib= d->pAttribute[i];  break;
        case eNodeText:      c.text  = d->pText[i];       break;

        case eNodeClear:     c.clear = d->pClear[i];      break;
    }
    return c;
}

void *XMLNode::enumContent(XMLNodeData *pEntry, int i, XMLElementType *nodeType)
{
    XMLElementType j=(XMLElementType)(pEntry->pOrder[i]&3);
    *nodeType=j;
    i=(pEntry->pOrder[i])>>2;
    switch (j)
    {
    case eNodeChild:      return pEntry->pChild[i].d;
    case eNodeAttribute:  return pEntry->pAttribute+i;
    case eNodeText:       return pEntry->pText[i];
    case eNodeClear:      return pEntry->pClear+i;

    }
    return NULL;
}

int XMLNode::nElement(XMLNodeData *pEntry)
{
    return pEntry->nChild+pEntry->nText+pEntry->nClear+pEntry->nAttribute;
}

// Creates an user friendly XML string from a given element with 
// appropriate white space and carriage returns.
//
// This recurses through all subnodes then adds contents of the nodes to the
// string.
int XMLNode::CreateXMLStringR(XMLNodeData *pEntry, LPTSTR lpszMarker, int nFormat)
{
    int nResult = 0;
    int cb;
    int cbElement;
    int nIndex;
    int nChildFormat;
    int bHasChildren = FALSE;
    int i;
    XMLAttribute * pAttr;

    assert(pEntry);

#define LENSTR(lpsz) (lpsz ? _tcslen(lpsz) : 0)

    // If the element has no name then assume this is the head node.
    cbElement = (int)LENSTR(pEntry->lpszName);

    if (cbElement)
    {
        // "<elementname "
        cb = nFormat == -1 ? 0 : nFormat;

        if (lpszMarker)
        {
            if (cb) memset(lpszMarker, INDENTCHAR, sizeof(TCHAR)*cb);
            nResult = cb;
            lpszMarker[nResult++]=_T('<');
            if (pEntry->isDeclaration) lpszMarker[nResult++]=_T('?');
            _tcscpy(&lpszMarker[nResult], pEntry->lpszName);
            nResult+=cbElement;
            lpszMarker[nResult++]=_T(' ');

        } else 
        {
            nResult+=cbElement+2+cb;
            if (pEntry->isDeclaration) nResult++;
        }
        
        // Enumerate attributes and add them to the string
        nIndex = pEntry->nAttribute; pAttr=pEntry->pAttribute;
        for (i=0; i<nIndex; i++)
        {
            // "Attrib
            cb = (int)LENSTR(pAttr->lpszName);
            if (cb)
            {
                if (lpszMarker) _tcscpy(&lpszMarker[nResult], pAttr->lpszName);
                nResult += cb;
                // "Attrib=Value "
                cb = (int)LENSTR(pAttr->lpszValue);
                if (cb)
                {
                    if (lpszMarker)
                    {
                        lpszMarker[nResult]=_T('=');
                        _tcscpy(&lpszMarker[nResult+1],pAttr->lpszValue);                  
                    }
                    nResult+=cb+1;
                }
                if (lpszMarker) lpszMarker[nResult] = _T(' ');
                nResult++;              
            }
            pAttr++;
        }
        
        bHasChildren=(pEntry->nAttribute!=nElement(pEntry));
        if (pEntry->isDeclaration)
        {
            if (lpszMarker) 
            {
                lpszMarker[nResult-1]=_T('?');
                lpszMarker[nResult]=_T('>');
            }
            nResult++;
            if (nFormat!=-1)
            {
                if (lpszMarker) lpszMarker[nResult]=_T('\n');
                nResult++;
            }
        } else
            // If there are child nodes we need to terminate the start tag
            if (bHasChildren)
            {   
                if (lpszMarker) lpszMarker[nResult-1]=_T('>');
                if (nFormat!=-1)
                {
                    if (lpszMarker) lpszMarker[nResult]=_T('\n');
                    nResult++;
                }
            } else nResult--;
    }

    // Calculate the child format for when we recurse.  This is used to 
    // determine the number of spaces used for prefixes.
    if (nFormat!=-1)
    {
        if (cbElement) nChildFormat=nFormat+1;
        else nChildFormat=nFormat;
    }

    // Enumerate through remaining children
    nIndex = nElement(pEntry);
    XMLElementType nodeType;
    void *pChild;
    for (i=0; i<nIndex; i++)
    {
        pChild=enumContent(pEntry, i, &nodeType);
        switch(nodeType)
        {
        // Text nodes
        case eNodeText:
            // "Text"
            cb = (int)LENSTR((LPTSTR)pChild);
            if (cb)
            {
                if (nFormat!=-1)
                {
                    if (lpszMarker)
                    {
                        memset(&lpszMarker[nResult],INDENTCHAR,sizeof(TCHAR)*(nFormat + 1));                       
                        _tcscpy(&lpszMarker[nResult+nFormat+1],(LPTSTR)pChild);
                        lpszMarker[nResult+nFormat+1+cb]=_T('\n');
                    }
                    nResult+=cb+nFormat+2;
                } else
                {
                    if (lpszMarker)_tcscpy(&lpszMarker[nResult], (LPTSTR)pChild);
                    nResult += cb;
                }
            }
            break;


        // Clear type nodes
        case eNodeClear:
            // "OpenTag"
            cb = (int)LENSTR(((XMLClear*)pChild)->lpszOpenTag);
            if (cb)
            {
                if (nFormat!=-1)
                {
                    if (lpszMarker)
                    {
                        memset(&lpszMarker[nResult], INDENTCHAR, sizeof(TCHAR)*(nFormat + 1));
                        _tcscpy(&lpszMarker[nResult+nFormat+1], ((XMLClear*)pChild)->lpszOpenTag);
                    }
                    nResult+=cb+nFormat+1;
                }
                else
                {
                    if (lpszMarker)_tcscpy(&lpszMarker[nResult], ((XMLClear*)pChild)->lpszOpenTag);
                    nResult += cb;
                }               
            }

            // "OpenTag Value"
            cb = (int)LENSTR(((XMLClear*)pChild)->lpszValue);
            if (cb)
            {
                if (lpszMarker) _tcscpy(&lpszMarker[nResult], ((XMLClear*)pChild)->lpszValue);
                nResult += cb;
            }

            // "OpenTag Value CloseTag"
            cb = (int)LENSTR(((XMLClear*)pChild)->lpszCloseTag);
            if (cb)
            {
                if (lpszMarker) _tcscpy(&lpszMarker[nResult], ((XMLClear*)pChild)->lpszCloseTag);
                nResult += cb;
            }

            if (nFormat!=-1)
            {
                if (lpszMarker) lpszMarker[nResult] = _T('\n');
                nResult++;
            }
            break;

        // Element nodes
        case eNodeChild:

            // Recursively add child nodes
            nResult += CreateXMLStringR((XMLNodeData*)pChild, 
                lpszMarker ? lpszMarker + nResult : 0, nChildFormat);
            break;
        default: break;
        }
    }

    if ((cbElement)&&(!pEntry->isDeclaration))
    {
        // If we have child entries we need to use long XML notation for 
        // closing the element - "<elementname>blah blah blah</elementname>"
        if (bHasChildren)
        {
            // "</elementname>\0"
            if (lpszMarker)
            {
                if (nFormat != -1)
                {
                    if (nFormat)
                    {
                        memset(&lpszMarker[nResult], INDENTCHAR,sizeof(TCHAR)*nFormat);
                        nResult+=nFormat;
                    }
                }       

                _tcscpy(&lpszMarker[nResult], _T("</"));
                nResult += 2;
                _tcscpy(&lpszMarker[nResult], pEntry->lpszName);
                nResult += cbElement;
                
                if (nFormat == -1)
                {
                    _tcscpy(&lpszMarker[nResult], _T(">"));
                    nResult++;
                } else
                {
                    _tcscpy(&lpszMarker[nResult], _T(">\n"));
                    nResult+=2;
                }
            } else
            {
                if (nFormat != -1) nResult+=cbElement+4+nFormat;
                else nResult+=cbElement+3;
            }       
        } else 
        {
            // If there are no children we can use shorthand XML notation - 
            // "<elementname/>"
            // "/>\0"
            if (lpszMarker)
            {
                if (nFormat == -1)
                {
                    _tcscpy(&lpszMarker[nResult], _T("/>")); 
                    nResult += 2;
                }
                else
                {
                    _tcscpy(&lpszMarker[nResult], _T("/>\n")); 
                    nResult += 3;
                }
            }
            else
            {
                nResult += nFormat == -1 ? 2 : 3;
            }
        }
    }

    return nResult;
}

#undef LENSTR

// Create an XML string from the head element.
// @param       XMLElement * pHead      - head element
// @param       int nFormat             - 0 if no formatting is required 
//                                        otherwise nonzero for formatted text
//                                        with carriage returns and indentation.
// @param       int *pnSize             - [out] pointer to the size of the 
//                                        returned string not including the 
//                                        NULL terminator.
//
// @return      LPTSTR                  - Allocated XML string, you must free
//                                        this with free().
LPTSTR XMLNode::createXMLString(int nFormat, int *pnSize)
{
    LPTSTR lpszResult = NULL;
    int cbStr;

    // Recursively Calculate the size of the XML string
    nFormat = nFormat ? 0 : -1;
    cbStr = CreateXMLStringR(d, 0, nFormat);
    assert(cbStr);
    // Alllocate memory for the XML string + the NULL terminator and
    // create the recursively XML string.
    lpszResult=(LPTSTR)malloc(cbStr+1);
    CreateXMLStringR(d, lpszResult, nFormat);
    if (pnSize) *pnSize = cbStr;
    return lpszResult;
}

XMLNode::~XMLNode()

{
    destroyCurrentBuffer(d);
};

void XMLNode::destroyCurrentBuffer(XMLNodeData *d)
{
    if (!d) return;
    (d->ref_count) --;
    if (d->ref_count==0)
    {
        int i=0;

        if (d->pParent)
        {
            XMLNode *pa=d->pParent->d->pChild;
            while (((void*)(pa[i].d))!=((void*)d)) i++; 
            d->pParent->d->nChild--;
            memmove(pa+i,pa+i+1,(d->pParent->d->nChild-i)*sizeof(XMLNode));
            removeOrderElement(d->pParent->d,eNodeChild,i);
        }

        for(i=0; i<d->nChild; i++) { d->pChild[i].d->pParent=NULL; destroyCurrentBuffer(d->pChild[i].d); }
        free(d->pChild);
        for(i=0; i<d->nText; i++) free(d->pText[i]);
        free(d->pText);
        for(i=0; i<d->nClear; i++) free(d->pClear[i].lpszValue);
        free(d->pClear);
        for(i=0; i<d->nAttribute; i++) 
        {
            free(d->pAttribute[i].lpszName);
            if (d->pAttribute[i].lpszValue) free(d->pAttribute[i].lpszValue);
        }
        free(d->pAttribute);
        free(d->pOrder);
        free(d->lpszName);
        free(d);
    }
}

XMLNode& XMLNode::operator=( const XMLNode& A )
{
    // shallow copy
    if (this != &A)
    {
        destroyCurrentBuffer(d);
        d=A.d;
        if (d) (d->ref_count) ++ ;
    }
    return *this;
}

XMLNode::XMLNode(const XMLNode &A)
{
    // shallow copy
    d=A.d;
    if (d) (d->ref_count)++ ;
}

int XMLNode::nChildNode(LPCTSTR name)
{
    int cbchild=(int)_tcslen(name),i,j=0,n=d->nChild;
    XMLNode *pc=d->pChild;
    for (i=0; i<n; i++)
    {
        if (_tcsnicmp(pc->d->lpszName, name, cbchild)==0) j++;
        pc++;
    }
    return j;
}

XMLNode XMLNode::getChildNode(LPCTSTR name, int *j)
{
    int cbchild=(int)_tcslen(name),i=0,n=d->nChild;
    if (j) i=*j;
    XMLNode *pc=d->pChild+i;
    for (; i<n; i++)
    {
        if (_tcsnicmp(pc->d->lpszName, name, cbchild)==0) 
        {
            if (j) *j=i+1;
            return *pc;
        }
        pc++;
    }
    return emptyXMLNode;
}

XMLNode XMLNode::getChildNode(LPCTSTR name, int j)
{
    int i=0; 
    while (j-->0) getChildNode(name,&i);
    return getChildNode(name,&i);
}

// Find an attribute on an node.
LPTSTR XMLNode::getAttribute(LPCTSTR lpszAttrib, int *j)
{
    int cbAttrib=(int)_tcslen(lpszAttrib),i=0,n=d->nAttribute;
    if (j) i=*j; 
    XMLAttribute *pAttr=d->pAttribute+i;
    for (; i<n; i++)
    {
        if (_tcsnicmp(pAttr->lpszName, lpszAttrib, cbAttrib)==0) 
        {
            if (j) *j=i+1;
            return pAttr->lpszValue;
        }
        pAttr++;
    }
    return NULL;
}

char XMLNode::isAttributeSet(LPCTSTR lpszAttrib)
{
    int cbAttrib=(int)_tcslen(lpszAttrib),i,n=d->nAttribute;
    XMLAttribute *pAttr=d->pAttribute;
    for (i=0; i<n; i++)
    {
        if (_tcsnicmp(pAttr->lpszName, lpszAttrib, cbAttrib)==0) 
        {
            return TRUE;
        }
        pAttr++;
    }
    return FALSE;
}

LPTSTR XMLNode::getAttribute(LPCTSTR name, int j)
{
    int i=0; 
    while (j-->0) getAttribute(name,&i);
    return getAttribute(name,&i);
}

LPCTSTR XMLNode::getName(){ return d->lpszName;   }
int XMLNode::nText()      { return d->nText;      }
int XMLNode::nChildNode() { return d->nChild;     }
int XMLNode::nAttribute() { return d->nAttribute; }
int XMLNode::nClear()     { return d->nClear;     }
XMLClear     XMLNode::getClear     (int i) { if (i>=d->nClear    ) return emptyXMLClear;     return d->pClear[i];     }
XMLAttribute XMLNode::getAttribute (int i) { if (i>=d->nAttribute) return emptyXMLAttribute; return d->pAttribute[i]; }
LPCTSTR      XMLNode::getText      (int i) { if (i>=d->nText     ) return NULL;              return d->pText[i];      }
XMLNode      XMLNode::getChildNode (int i) { if (i>=d->nChild    ) return emptyXMLNode;      return d->pChild[i];     }
char         XMLNode::isDeclaration(     ) { return d->isDeclaration; }
char         XMLNode::isEmpty      (     ) { return (d==NULL);        }
int          XMLNode::nElement     (     ) { return d->nChild+d->nText+d->nClear+d->nAttribute; }
