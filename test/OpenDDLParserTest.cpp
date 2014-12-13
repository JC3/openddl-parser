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
#include "gtest/gtest.h"

#include <openddlparser\OpenDDLParser.h>

BEGIN_ODDLPARSER_NS

static char *findEnd( char *in, size_t &len ) {
    len = strlen( in );
    char *end( &in[ len ] + 1 );

    return end;
}

class OpenDDLParserTest : public testing::Test {
protected:
};

TEST_F(OpenDDLParserTest, isUpperCaseTest) {
    char val = 'a';
    EXPECT_TRUE(isLowerCase<char>(val));
    val = 'c';
    EXPECT_TRUE(isLowerCase<char>(val));
    val = 'z';
    EXPECT_TRUE(isLowerCase<char>(val));
    val = 'A';
    EXPECT_FALSE(isLowerCase<char>(val));
    val = '0';
    EXPECT_FALSE(isLowerCase<char>(val));
}

TEST_F(OpenDDLParserTest, isLowerCaseTest) {
    char val = 'A';
    EXPECT_TRUE(isUpperCase<char>(val));
    val = 'C';
    EXPECT_TRUE(isUpperCase<char>(val));
    val = 'Z';
    EXPECT_TRUE(isUpperCase<char>(val));
    val = 'a';
    EXPECT_FALSE(isUpperCase<char>(val));
    val = '0';
    EXPECT_FALSE(isUpperCase<char>(val));
}

TEST_F(OpenDDLParserTest, isNumericTest) {
    char val1 = '0';
    EXPECT_TRUE( isNumeric<char>(val1) );
    char val2 = '9';
    EXPECT_TRUE(isNumeric<char>(val2));
    char val3 = '4';
    EXPECT_TRUE(isNumeric<char>(val3));
    char not = 'a';
    EXPECT_FALSE(isNumeric<char>(not));
}

TEST_F( OpenDDLParserTest, isSeparatorTest ) {
    char val;
    val = ' ';
    EXPECT_TRUE( isSeparator( val ) );
    val = '\n';
    EXPECT_TRUE( isSeparator( val ) );
    val = '\t';
    EXPECT_TRUE( isSeparator( val ) );
    val = '1';
    EXPECT_FALSE( isSeparator( val ) );
    val = 'a';
    EXPECT_FALSE( isSeparator( val ) );
    val = 'Z';
    EXPECT_FALSE( isSeparator( val ) );
}

TEST_F( OpenDDLParserTest, getNextSeparatorTest ) {
    char tokenlist[] = "abc, \na";
    char *in( &tokenlist[ 0 ] );
    char *end( &tokenlist[ strlen( "abc, \na" ) ] );

    bool res( true );
    res = isSeparator( *in );
    EXPECT_FALSE( res );
    in = getNextSeparator( in, end );
    res = isSeparator( *in );
    EXPECT_TRUE( res );
    in = getNextSeparator( in, end );
    res = isSeparator( *in );
    EXPECT_TRUE( res );
}

TEST_F( OpenDDLParserTest, PrimDataAllocTest ) {
    PrimData *data = PrimDataAllocator::allocPrimData( ddl_bool );
    EXPECT_NE( nullptr, data );
    PrimDataAllocator::releasePrimData( &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, PrimDataAccessBoolTest ) {
    PrimData *data = PrimDataAllocator::allocPrimData( ddl_bool );
    EXPECT_NE( nullptr, data );
    data->setBool( true );
    EXPECT_EQ( true, data->getBool() );
    data->setBool( false );
    EXPECT_EQ( false, data->getBool() );
    PrimDataAllocator::releasePrimData( &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, createTest ) {
    bool success( true );
    try {
        OpenDDLParser myParser;
        EXPECT_EQ( nullptr, myParser.getRoot() );
    } catch( ... ) {
        success = false;
    }
    EXPECT_TRUE( success );
}

TEST_F( OpenDDLParserTest, parseIdentifierTest ) {
    int res( 0 );
    size_t len1( 0 );
    char name1[] = "testname", *end1( findEnd( name1, len1 ) );

    Identifier *id( nullptr );
    char *in = OpenDDLParser::parseIdentifier( name1, end1, &id );
    EXPECT_TRUE( id != nullptr );
    res = strncmp( id->m_buffer, name1, len1 );
    EXPECT_EQ( 0, res );
    
    size_t len2( 0 );
    char name2[] = " testname ", *end2( findEnd( name2, len2 ) );
    in = OpenDDLParser::parseIdentifier( name2, end2, &id );
    EXPECT_TRUE( id != nullptr );
    const size_t tokenLen( strlen( id->m_buffer ) );
    res = strncmp( id->m_buffer, name1, tokenLen );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLParserTest, parseNameTest ) {
    int res( 0 );
    size_t len1( 0 );
    char name1[] = "$testname", *end1( findEnd( name1, len1 ) );

    Name *name(  nullptr );
    char *in = OpenDDLParser::parseName( name1, end1, &name );
    EXPECT_TRUE( name != nullptr );
}

TEST_F( OpenDDLParserTest, parsePrimitiveDataTypeTest ) {
    PrimData *data1( nullptr );
    char *in = OpenDDLParser::parsePrimitiveDataType( nullptr, nullptr, &data1 );
    EXPECT_EQ( nullptr, data1 );
    EXPECT_EQ( nullptr, in );

    size_t len1( 0 );
    char token1[] = "float", *end1( findEnd( token1, len1 ) );
    in = OpenDDLParser::parsePrimitiveDataType( token1, end1, &data1 );
    EXPECT_NE( nullptr, data1 );

    size_t len2( 0 );
    PrimData *data2( nullptr );
    char invalidToken[] = "foat", *end2( findEnd( token1, len2 ) );
    in = OpenDDLParser::parsePrimitiveDataType( invalidToken, end2, &data2 );
    EXPECT_EQ( nullptr, data2 );
}
 
TEST_F( OpenDDLParserTest, parsePrimitiveDataTypeWithArrayTest ) {
    PrimData *data( nullptr );
    size_t len1( 0 );
    char token[] = "float[3]", *end( findEnd( token, len1 ) );

    const size_t len( strlen( token ) );
    char *in = OpenDDLParser::parsePrimitiveDataType( token, end, &data );
    ASSERT_NE( nullptr, data );
    EXPECT_EQ( ddl_float, data->m_type );
    EXPECT_EQ( 12, data->m_size );
    EXPECT_NE( nullptr, data->m_data );
}

TEST_F( OpenDDLParserTest, parsePrimitiveDataTypeWithInvalidArrayTest ) {
    PrimData *data( nullptr );
    size_t len1( 0 );
    char token1[] = "float[3", *end( findEnd( token1, len1 ) );

    char *in = OpenDDLParser::parsePrimitiveDataType( token1, end, &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, parseReferenceTest ) {
    bool success( true );
    size_t len1( 0 );
    char token1[] = "ref { $name1, %name2 }", *end( findEnd( token1, len1 ) );

    std::vector<Name*> names;
    success = OpenDDLParser::parseReference( token1, end, names );
    EXPECT_TRUE( success );
    EXPECT_EQ( 2, names.size() );

    int res( 0 );
    Name *name( nullptr );
    name = names[ 0 ];
    EXPECT_NE( nullptr, name );
    EXPECT_EQ( GlobalName, name->m_type );
    res = strncmp( name->m_id->m_buffer, "$name1", strlen( "$name1" ) );
    EXPECT_EQ( 0, res );

    name = names[ 1 ];
    EXPECT_NE( nullptr, name );
    EXPECT_EQ( LocalName, name->m_type );
    res = strncmp( name->m_id->m_buffer, "%name2", strlen( "%name2" ) );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLParserTest, parseBooleanLiteralTest ) {
    char *in( nullptr );
    PrimData *data( nullptr );
    bool success( true );
    size_t len1( 0 );
    char token1[] = "true", *end1( findEnd( token1, len1 ) );
    in = OpenDDLParser::parseBooleanLiteral( token1, end1, &data );
    EXPECT_EQ( ddl_bool, data->m_type );
    EXPECT_EQ( true, data->getBool() );

    size_t len2( 0 );
    char token2[] = "false", *end2( findEnd( token2, len2 ) );
    in = OpenDDLParser::parseBooleanLiteral( token2, end2, &data );
    EXPECT_EQ( ddl_bool, data->m_type );
    EXPECT_EQ( false, data->getBool() );

    size_t len3( 0 );
    char token3[] = "fallse", *end3( findEnd( token3, len3 ) );
    in = OpenDDLParser::parseBooleanLiteral( token3, end3, &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, parseIntegerLiteralTest ) {
    char *in( nullptr );
    size_t len1( 0 );
    PrimData *data( nullptr );

    char token1[] = "1", *end1( findEnd( token1, len1 ) );
    in = OpenDDLParser::parseIntegerLiteral( token1, end1, &data );
    EXPECT_EQ( ddl_int32, data->m_type );
    EXPECT_EQ( 1, data->getInt32() );

    size_t len2( 0 );
    char token2[] = "aaa", *end2( findEnd( token2, len2 ) );
    in = OpenDDLParser::parseIntegerLiteral( token2, end2, &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, parseInvalidIntegerLiteralTest ) {
    size_t len1( 0 );
    PrimData *data( nullptr );
    char token1[] = "1", *end1( findEnd( token1, len1 ) );
    char *in( token1 );
    char *out = OpenDDLParser::parseIntegerLiteral( token1, end1, &data, ddl_float );
    EXPECT_EQ( out, in );
}

END_ODDLPARSER_NS
