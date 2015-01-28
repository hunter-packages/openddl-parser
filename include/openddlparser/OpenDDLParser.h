/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2014 Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once
#ifndef OPENDDLPARSER_OPENDDLPARSER_H_INC
#define OPENDDLPARSER_OPENDDLPARSER_H_INC

#include <openddlparser/OpenDDLCommon.h>
#include <openddlparser/DDLNode.h>
#include <openddlparser/OpenDDLParserUtils.h>
#include <openddlparser/Value.h>

#include <vector>
#include <string>

BEGIN_ODDLPARSER_NS

class DDLNode;
class Value;

struct Identifier;
struct Reference;
struct Property;

template<class T>
inline
T *getNextToken( T *in, T *end ) {
    while( ( isSpace( *in ) || isNewLine( *in ) || ',' == *in ) && ( in != end ) ) {
        in++;
    }
    return in;
}

enum NameType {
    GlobalName,
    LocalName
};

struct Name {
    NameType m_type;
    Identifier *m_id;

    Name( NameType type, Identifier *id )
    : m_type( type )
    , m_id( id ) {
        // empty
    }
};

struct Reference {
    size_t m_numRefs;
    Name **m_referencedName;

    Reference( size_t numrefs, Name **names )
    : m_numRefs( numrefs )
    , m_referencedName( names ) {
        // empty
    }
};

struct Identifier {
    size_t m_len;
    char *m_buffer;

    Identifier( size_t len, char *buffer )
    : m_len( len )
    , m_buffer( buffer ) {
        // empty
    }
};

struct Property {
    Identifier *m_id;
    Value *m_primData;
    Reference *m_ref;
    Property *m_next;

    Property( Identifier *id )
    : m_id( id )
    , m_primData( nullptr ) 
    , m_ref( nullptr )
    , m_next( nullptr ) {
        // empty
    }
};

enum LogSeverity {
    ddl_debug_msg = 0,
    ddl_info_msg,
    ddl_warn_msg,
    ddl_error_msg,
};

class DLL_ODDLPARSER_EXPORT OpenDDLParser {
public:
    typedef void( *logCallback )( LogSeverity severity, const std::string &msg );

public:
    OpenDDLParser();
    OpenDDLParser( char *buffer, size_t len, bool ownsIt = false );
    ~OpenDDLParser();
    void setLogCallback( logCallback callback );
    logCallback getLogCallback() const;
    void setBuffer( char *buffer, size_t len, bool ownsIt = false );
    char *getBuffer() const;
    size_t getBufferSize() const;
    void clear();
    bool parse();
    char *parseNextNode( char *current, char *end );
    char *parseHeader( char *in, char *end );
    char *parseStructure( char *in, char *end );
    void pushNode( DDLNode *node );
    DDLNode *popNode();
    DDLNode *top();
    DDLNode *getRoot() const;

public: // static parser helpers
    static void normalizeBuffer( char *buffer, size_t len );
    static char *parseName( char *in, char *end, Name **name );
    static char *parseIdentifier( char *in, char *end, Identifier **id );
    static char *parsePrimitiveDataType( char *in, char *end, Value::ValueType &type, size_t &len );
    static char *parseReference( char *in, char *end, std::vector<Name*> &names );
    static char *parseBooleanLiteral( char *in, char *end, Value **boolean );
    static char *parseIntegerLiteral( char *in, char *end, Value **integer, Value::ValueType integerType = Value::ddl_int32 );
    static char *parseFloatingLiteral( char *in, char *end, Value **floating );
    static char *parseStringLiteral( char *in, char *end, Value **stringData );
    static char *parseHexaLiteral( char *in, char *end, Value **data );
    static char *parseProperty( char *in, char *end, Property **prop );
    static char *parseDataList( char *in, char *end, Value **data );
    static char *parseDataArrayList( char *in, char *end, Value **data );
    static const char *getVersion();

private:
    OpenDDLParser( const OpenDDLParser & );
    OpenDDLParser &operator = ( const OpenDDLParser & );

private:
    logCallback m_logCallback;
    bool m_ownsBuffer;
    char *m_buffer;
    size_t m_len;
    DDLNode *m_root;
    typedef std::vector<DDLNode*> DDLNodeStack;
    DDLNodeStack m_stack;
};

END_ODDLPARSER_NS

#endif // OPENDDLPARSER_OPENDDLPARSER_H_INC
