# include <iostream>
# include <string>
# include <vector>
# include <cstdlib> 
# include <cstdio>
# include <cstring>
# include <sstream>
# include <iomanip>
# include <stack>
# include <math.h>
# include <queue>

using namespace std;

# define eof         0             
# define Identifier  1      
# define Constant    2     
// e.g., 35, 35.67, 'a', "Hi, there", true, false, .35, 35., 0023
# define INT         3
// int
# define FLOAT       4
// float
# define CHAR        5
// char
# define BOOL        6
// bool
# define STRING      7
// string <----------- 注意全小寫！
# define VOID        8
// void
# define IF          9
// if
# define ELSE        10
// else
# define WHILE       11
// while
# define DO          12
// do
# define RETURN      13
// return
# define LP          14
// '('  left parentheses
# define RP          15
// ')' right parentheses
# define LSB         16
// '[' left square brackets
# define RSB         17
// ']' right square brackets
# define LCB         18
// '{' left curly brackets
# define RCB         19
//  '}'   right curly brackets
# define PLUS        20
// '+'
# define SUB         21
// '-' 
# define MULT        22
// '*'
# define DIV         23
// '/' 
# define remainder   24 
// '%' remainder operator
# define XOR         25
// '^' bitwise XOR operator
# define GT          26
// '>' greater than operator
# define LT          27
// '<' less than operator
# define GE          28
// >= 
# define LE          29
// <= 
# define EQ          30
// ==
# define NEQ         31
// !=
# define BAnd        32
// '&' bitwise AND operator
# define BOr         33
// '|' bitwise OR operator
# define assignment  34
// '=' 
# define EM          35
// '!'
# define AND         36
// && 
# define OR          37
// ||
# define PE          38
// +=
# define ME          39
// -= 
# define TE          40
// *= 
# define DE          41
// /=
# define RE          42
// %=
# define PP          43
// ++ 
# define MM          44
// --
# define RS          45
// >> 
# define LS          46
// << 
# define SEMICOLON   47
// ';'
# define COMMA       48
// ','
# define QM          49
// '?'
# define COLON       50
// ':'
# define DQ          51
// '"'
# define SQ          52
// '''
# define nextline    53
// 換行 
# define Unrecognize 54



struct Line
{
  vector<char> ch ; 
};

struct Def 
{
  string name ;
  double value;
  bool isFloat ;
  int num ;
} ;

struct Error 
{
  string token ;
  int errorType ;
  int insStart ;
  int errorLine ;
} ;

struct Token
{
  string token;
  int type ;
  int row ;
};

struct Fun {
  string name ;
  vector<Token> content ;
};

struct Var {
  int type ;
  string name ;
  string size ;
  string value ;
  bool first ;
  bool haveSize ;
  vector<string> array ;
};



struct Formula
{
  string value;
  int type;
};

class Interpreter 
{
  private:
    
  vector<Line> mlines ;
  Line mgTemp ;
  vector<Def> mdef ;
  vector<Token> msToken ;
  vector<Def> mNotDEF ;
  vector<Fun> mFunDef ;
  vector<Var> mVarDef ;
  vector<Var> mFunVarDef ;
  vector<string> mTempFun ;                // 要印出的 
  vector<string> mTempVar ;                // 要印出的 
  vector<Var> mVarTemp ;                   // 要定義的 
  Error mError ;
  vector<Formula> mOutput ;
  Formula mFirstValue ;
  int mtokenNum ;
  bool mNotDef ;
  bool mQuit ;
  bool mFirst ;
    
  public :
    
  void Read() 
  {
    char ch;
    
      
    while ( cin.peek() != EOF ) {

      ch = cin.get() ;

      while ( ch != '\n' && ch != EOF ) {
        mgTemp.ch.push_back( ch ) ;
        ch = cin.get() ;
      } // while( ch != '\n' )
        
      mgTemp.ch.push_back( ch ) ;
      mlines.push_back( mgTemp ) ;
      mgTemp.ch.clear() ;
      
        
    } // while( ch != EOF )
      
    mgTemp.ch.push_back( EOF ) ;  
    mlines.push_back( mgTemp ) ;
      
  } // Read()
  
  void Print2() {
      
    for ( int i = 0 ; i < msToken.size() ; i++ ) {
      cout << msToken[i].type << "  " << msToken[i].token << endl ;
    } // for()
      
  } // Print2()

  char UnRecognizedChar( int &row, int &column ) {
            
    char ch = mlines[row].ch[column] ;
    while ( mlines[row].ch[column] != '\n' ) {        
      column++ ;        
    } // while()
            
    return ch;
               
  } // UnRecognizedChar()
        
  Token FirstConstant( int &row, int &column )                   // 判斷Constant 
  {   
    Token token;
    string temp = "";
    bool dot = false ;
            
    if ( mlines[row].ch[column] == '.' ) {              // 一開始遇到. 
      temp.push_back( mlines[row].ch[column] ) ;
      token.token = token.token + temp ;
      dot = true ;
      column++;
    } // if( mlines[row].ch[column] == '.' )
            
    temp = "" ;
    if ( mlines[row].ch[column] >= '0' && mlines[row].ch[column] <= '9' )   {         // 數字 
  
      while ( mlines[row].ch[column] >= '0' && mlines[row].ch[column] <= '9' )  {      // 數字 
                
        temp.push_back( mlines[row].ch[column] );
        token.token = token.token + temp ;
      
        if ( ( column+1 ) < mlines[row].ch.size()  )    {
    
          if ( mlines[row].ch[column+1] == '.' && dot == false ) {                        // 有小數點 
            temp = "" ;
            temp.push_back( mlines[row].ch[column+1] );
            token.token = token.token + temp ;
            column++;
            dot = true ;
          } // if( mlines[row].ch[column+1] == '.' )
          else if ( mlines[row].ch[column+1] == '.' && dot == true ) {
            token.type = Constant ;
            token.row = row ;
            column++;
            return token ;    
          } // else if
        } // if
      
        column++;
        temp = "";    
        
      } // while()
    } // if
    else if ( mlines[row].ch[column] == 34 ) {  // "
       
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      temp = "" ;
      column++ ;                                 // 跳過" 
      
      while ( mlines[row].ch[column] != 34 )  {      // "
                
        temp.push_back( mlines[row].ch[column] );
        token.token = token.token + temp ;
        column++;
        temp = "";    
        
      } // while()
      
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      column++ ;  
    } // else if
    else {
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      column++ ;                                 // 跳過' 
      
      while ( mlines[row].ch[column] != 39 )  {      // ' 
                
        temp.push_back( mlines[row].ch[column] );
        token.token = token.token + temp ;
      
        column++;
        temp = "";    
        
      } // while()  
      
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      column++ ;  
    } // else
  
    token.type = Constant ;
    token.row = row ;
    return token ;
    
  } // FirstConstant()
        
  Token IDToken( int &row, int &column )                         // 判斷ID 
  {
    Token token ;
    string temp ;
    bool quit = false ;
            
    while ( ( ( mlines[row].ch[column] >= 'A' && mlines[row].ch[column] <= 'Z' ) 
              || ( mlines[row].ch[column] >= 'a' && mlines[row].ch[column] <= 'z' ) 
              || ( mlines[row].ch[column] >= '0' && mlines[row].ch[column] <= '9' )
              || mlines[row].ch[column] == '_' ) && quit == false ) {
                        
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
                
      column++;
      temp = "";
      
      if ( column > mlines[row].ch.size() ) quit = true ;          
                
    } // while()
    
    if ( token.token == "int" ) {
      token.type = INT ;
    }  // if
    else if ( token.token == "float" ) {
      token.type = FLOAT ;
    } // else if
    else if ( token.token == "char" ) {
      token.type = CHAR ;  
    } // else if
    else if ( token.token == "bool" ) {
      token.type = BOOL ;
    } // else if
    else if ( token.token == "string" ) {
      token.type = STRING ;
    } // else if
    else if ( token.token == "void" ) {
      token.type = VOID ;
    } // else if
    else if ( token.token == "if" ) {
      token.type = IF ;
    } // else if
    else if ( token.token == "else" ) {
      token.type = ELSE ;
    } // else if
    else if ( token.token == "while" ) {
      token.type = WHILE ;
    } // else if
    else if ( token.token == "do" ) {
      token.type = DO ;
    } // else if
    else if ( token.token == "return" ) {
      token.type = RETURN ;
    } // else if
    else if ( token.token == "true" ) {
      token.type = Constant ;
    } // else if
    else if ( token.token == "false" ) {
      token.type = Constant ;
    } // else if
    else {
      token.type = Identifier;    
    } // else 
            
    token.row = row ;
    return token;
            
  } // IDToken()
        
  Token FirstThan( int &row, int &column ) {                     // 判斷<,> 
    
    Token token ;
    string temp ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '<' ) {           
                
        if ( mlines[row].ch[column+1] == '<' ) {            // <<
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = LS ;
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '<' )
        else if ( mlines[row].ch[column+1] == '=' ) {      // <=
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type =  LE ;
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // else if ( mlines[row].ch[column+1] == '=' )
        
      } // if( mlines[row].ch[column+1] == '>' )
      else {
                
        if ( mlines[row].ch[column+1] == '>' ) {           // >>
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = RS ;
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column+1] == '=' )
        else if ( mlines[row].ch[column+1] == '=' ) {      // >=
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type =  GE ;
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // else if ( mlines[row].ch[column+1] == '=' )
        
      } // else
    } // if
    
    if ( mlines[row].ch[column] == '<' ) {          // <
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = LT ;
    } // if
    else {
      temp.push_back( mlines[row].ch[column] );      // >
      token.token = token.token + temp ;
      token.type = GT ;
    } // else  
    
    
    column++;
    token.row = row ;
    return token ;
            
  } // FirstThan()
  
  Token PlusSub( int &row, int &column ) {                       // 判斷+,- 
    
    Token token ;
    string temp ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '+' ) {           
                
        if ( mlines[row].ch[column+1] == '+' ) {                 // ++
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = PP;
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '+' )
        else if ( mlines[row].ch[column+1] == '=' ) {            // +=
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type =  PE ;
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // else if ( mlines[row].ch[column+1] == '=' )
        
      } // if( mlines[row].ch[column+1] == '>' )
      else {
                
        if ( mlines[row].ch[column+1] == '-' ) {                 // --
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type =  MM;
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '-' )
        else if ( mlines[row].ch[column+1] == '=' ) {            // -=
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type =  ME;
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // else if ( mlines[row].ch[column+1] == '=' )
        
      } // else
    } // if
    
    if ( mlines[row].ch[column] == '+' ) {                       // +
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = PLUS ;
    } // if
    else {
      temp.push_back( mlines[row].ch[column] );                  // -
      token.token = token.token + temp ;
      token.type = SUB ;
    } // else  
    
    
    column++;
    token.row = row ;
    return token ;
            
  } // PlusSub()
        
  Token FirstColon( int &row, int &column ) {                    // 判斷 :
             
    Token token ;
    string temp ;
                            
    temp.push_back( mlines[row].ch[column] );                    //  :
    token.token = token.token + temp ;
    token.type =  COLON ;
    
            
    column++;
    token.row = row ;
    return token;
            
  } // FirstColon()
        
        
  Token IsComment( int &row, int &column ) {                     // 判斷 // or / 
            
    Token token ;
    string temp ;
    
    if ( ( column+1 ) < mlines[row].ch.size() )  {
      if ( mlines[row].ch[column+1] == '/' ) {                   //  // 
        while ( mlines[row].ch[column] != '\n' && mlines[row].ch[column] != EOF )
          column++ ;
      
        token.type = 0 ;
        return token;      
      } // if( mlines[row].ch[column+1] == '/' )
      else if ( mlines[row].ch[column+1] == '=' ) {              // /=
        temp.push_back( mlines[row].ch[column] );
        temp.push_back( mlines[row].ch[column+1] );
        token.token = token.token + temp ;
        token.type = DE ;
        column++;
        column++ ;
        token.row = row ;
        return token ;
      } // else if
    } // if
    
    
    temp.push_back( mlines[row].ch[column] );                    //  /
    token.token = token.token + temp ;
    token.type = DIV ;
    column++;
    token.row = row ;
    
    return token;
            
  } // IsComment()
        
  Token IsParentheses( int &row, int &column ) {                 // 判斷 (),[],{} 
    Token token ;
    string temp ;
             
    if ( mlines[row].ch[column] == '(' ) {           // (
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = LP;
    } // if
    else if ( mlines[row].ch[column] == ')' ) {
      temp.push_back( mlines[row].ch[column] );      // )
      token.token = token.token + temp ;
      token.type = RP ;
    } // else if
    else if ( mlines[row].ch[column] == '{' ) {
      temp.push_back( mlines[row].ch[column] );      // {
      token.token = token.token + temp ;
      token.type = LCB ;
    } // else if
    else if ( mlines[row].ch[column] == '}' ) {
      temp.push_back( mlines[row].ch[column] );      // }
      token.token = token.token + temp ;
      token.type = RCB ;
    } // else if
    else if ( mlines[row].ch[column] == '[' ) {
      temp.push_back( mlines[row].ch[column] );      // [
      token.token = token.token + temp ;
      token.type = LSB ;
    } // else if
    else if ( mlines[row].ch[column] == ']' ) {
      temp.push_back( mlines[row].ch[column] );      // ]
      token.token = token.token + temp ;
      token.type = RSB ;
    } // else if
    
    column++;
    token.row = row ;
    return token ;
    
  } // IsParentheses()
  
  Token IsMULT( int &row, int &column ) {                        // 判斷* 
    
    Token token ;
    string temp ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '*' ) {           
                
        if ( mlines[row].ch[column+1] == '=' ) {            // *=
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = TE ;                                 // *=
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '<' )
        
      } // if( mlines[row].ch[column+1] == '>' )
    } // if
  
    if ( mlines[row].ch[column] == '*' ) {                  // *
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = MULT ;
    } // if
    
    
    column++;
    token.row = row ;
    return token ;
            
  } // IsMULT()
  
  Token IsEQUAL( int &row, int &column ) {                       // 判斷 = or == 
    
    Token token ;
    string temp ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '=' ) {           
                
        if ( mlines[row].ch[column+1] == '=' ) {            // ==
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = EQ ;                                 // ==
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '<' )
        
      } // if( mlines[row].ch[column+1] == '>' )
    } // if
  
    if ( mlines[row].ch[column] == '=' ) {                  // =
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = assignment ;
    } // if
    
    column++;
    token.row = row ;
    return token ;
            
  } // IsEQUAL()
  
  Token IsRemainder( int &row, int &column ) {                   // 判斷 % or %= 
    
    Token token ;
    string temp ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '%' ) {           
                
        if ( mlines[row].ch[column+1] == '=' ) {                 // %=
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = RE ;                                      // %=
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '<' )
        
      } // if( mlines[row].ch[column+1] == '>' )
    } // if
  
    if ( mlines[row].ch[column] == '%' ) {                       // %
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = remainder ;
    } // if
    
    column++;
    token.row = row ;
    return token ;
            
  } // IsRemainder()
  
  Token IsExclamation( int &row, int &column ) {                 // 判斷 ! or != 
    
    Token token ;
    string temp ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '!' ) {           
                
        if ( mlines[row].ch[column+1] == '=' ) {                // !=
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = NEQ ;                                    // !=
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '<' )
        
      } // if( mlines[row].ch[column+1] == '>' )
    } // if
  
    if ( mlines[row].ch[column] == '!' ) {                      // !
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = EM ;
    } // if
    
    column++;
    token.row = row ;
    return token ;
            
  } // IsExclamation()
  
  Token IsAND( int &row, int &column ) {                         // 判斷 & or && 
    
    Token token ;
    string temp ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '&' ) {           
                
        if ( mlines[row].ch[column+1] == '&' ) {                 // &&
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = AND ;                                     // &&
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '<' )
        
      } // if( mlines[row].ch[column+1] == '>' )
    } // if
  
    if ( mlines[row].ch[column] == '&' ) {                       // &
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = BAnd ;
    } // if
    
    column++;
    token.row = row ;
    return token ;
            
  } // IsAND()
  
  Token IsOR( int &row, int &column ) {                          // 判斷 | or || 
    
    Token token ;
    string temp ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '|' ) {           
                
        if ( mlines[row].ch[column+1] == '|' ) {                 // ||
          temp.push_back( mlines[row].ch[column] );
          temp.push_back( mlines[row].ch[column+1] );
          token.token = token.token + temp ;
          token.type = OR ;                                     // ||
          column++;
          column++ ;
          token.row = row ;
          return token ;
        } // if( mlines[row].ch[column] == '<' )
        
      } // if( mlines[row].ch[column+1] == '>' )
    } // if
  
    if ( mlines[row].ch[column] == '|' ) {                       // |
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = BOr ;
    } // if
    
    column++;
    token.row = row ;
    return token ;
            
  } // IsOR()
  
  Token IsQUOTATION( int &row, int &column ) {                   // 判斷 " or '
    
    Token token ;
    string temp ;
    bool quit = false ;
    
    if ( column+1 < mlines[row].ch.size() ) {
          
      if ( mlines[row].ch[column] == '"' || mlines[row].ch[column] == 39 ) {       // " or '
      
        temp.push_back( mlines[row].ch[column] ) ;
        token.token = token.token + temp ;
        column++ ;
                
        while ( ( ( mlines[row].ch[column] >= 'A' && mlines[row].ch[column] <= 'Z' ) 
                  || ( mlines[row].ch[column] >= 'a' && mlines[row].ch[column] <= 'z' ) 
                  || ( mlines[row].ch[column] >= '0' && mlines[row].ch[column] <= '9' )
                  || mlines[row].ch[column] == '_' || mlines[row].ch[column] == '.' ) && quit == false ) {
                        
          temp.push_back( mlines[row].ch[column] );
          token.token = token.token + temp ;
                
          column++;
          temp = "";
      
          if ( column > mlines[row].ch.size() ) quit = true ;          
                
        } // while()
        
      } // if( mlines[row].ch[column+1] == '>' )
    } // if
  
    if ( mlines[row].ch[column] == '"' || mlines[row].ch[column] == 39 ) {                       // " or '
      temp.push_back( mlines[row].ch[column] );
      token.token = token.token + temp ;
      token.type = Constant ;
    } // if
    
    column++;
    token.row = row ;
    return token ;
            
  } // IsQUOTATION()
  
  
  void Scanner() 
  {
            
    bool quit = false ;
    int row = 0 ;
    int column = 0 ;
    char unRecognized ;
    string temp ;
    Token token ;
            
            
    while ( quit == false && row < mlines.size() ) {
           

      if ( mlines[row].ch[column] != ' ' && mlines[row].ch[column] != '\n' ) {          //  一個一個切TOKEN
            
                    
        if ( ( mlines[row].ch[column] >= '0' && mlines[row].ch[column] <= '9' ) 
             || mlines[row].ch[column] == 39 || mlines[row].ch[column] == 34           //  39是'   34是" 
             || mlines[row].ch[column] == '.' )                                         // Constant
        {
          msToken.push_back( FirstConstant( row, column ) )  ;
        } // if
        else if ( ( mlines[row].ch[column] >= 'A' && mlines[row].ch[column] <= 'Z' ) 
                  || ( mlines[row].ch[column] >= 'a' && mlines[row].ch[column] <= 'z' ) 
                  || mlines[row].ch[column] == '_' )                                   // ID
        {   
          msToken.push_back( IDToken( row, column ) )  ;
        } // else if
        else if ( mlines[row].ch[column] == '+' || mlines[row].ch[column] == '-' )     // 開頭是+,- 
        {
          msToken.push_back( PlusSub( row, column ) )  ;              
        } // else if
        else if ( mlines[row].ch[column] == '<' || mlines[row].ch[column] == '>' )     // 開頭是<,> 
        {
          msToken.push_back( FirstThan( row, column ) ) ; 
        } // else if
        else if ( mlines[row].ch[column] == ';' )                                      // 開頭是;
        {
          temp.push_back( mlines[row].ch[column] );
          token.token = token.token + temp ;
          token.type = SEMICOLON ;
          token.row = row ;
          msToken.push_back( token ) ;
          column++;    
        } // else if
        else if ( mlines[row].ch[column] == '{' || mlines[row].ch[column] == '}' 
                  || mlines[row].ch[column] == '[' || mlines[row].ch[column] == ']'
                  || mlines[row].ch[column] == '(' || mlines[row].ch[column] == ')' )   
        {        // 開頭是 (),{},[] 123 = {, 125 = }
          msToken.push_back( IsParentheses( row, column ) ) ;             
        } // else if
        else if ( mlines[row].ch[column] == '*' )                                       // * or *=
        {
          msToken.push_back( IsMULT( row, column ) ) ;
        } // else if
        else if ( mlines[row].ch[column] == '/' ) {                                     //  / or // or /=
          token = IsComment( row, column ) ;
          if ( token.type == DIV || token.type == DE )  
            msToken.push_back( token ) ;
        } // else if
        else if ( mlines[row].ch[column] == '=' )  {                                    // = or ==
          msToken.push_back( IsEQUAL( row, column ) ) ;
        }  // else if
        else if ( mlines[row].ch[column] == ':' ) {                                     // : 
          msToken.push_back( FirstColon( row, column ) )  ;
        } // else if
        else if ( mlines[row].ch[column] == '%' ) {                                     // % or %=
          msToken.push_back( IsRemainder( row, column ) )  ;
        } // else if
        else if ( mlines[row].ch[column] == '!' ) {                                     // ! or !=
          msToken.push_back( IsExclamation( row, column ) )  ;
        } // else if
        else if ( mlines[row].ch[column] == '&' ) {                                     // & or && 
          msToken.push_back( IsAND( row, column ) )  ;
        } // else if
        else if ( mlines[row].ch[column] == '|' ) {                                     // | or ||
          msToken.push_back( IsOR( row, column ) )  ;
        } // else if
        else if ( mlines[row].ch[column] == '?' ) {                                     // ?
          temp.push_back( mlines[row].ch[column] ) ;
          token.token = token.token + temp ;
          token.type =  QM ;
          token.row = row ;
          msToken.push_back( token ) ;
          column++;    
        } // else if
        else if ( mlines[row].ch[column] == ',' ) {                                     // ,
          temp.push_back( mlines[row].ch[column] ) ;
          token.token = token.token + temp ;
          token.type =  COMMA ;
          token.row = row ;
          msToken.push_back( token ) ;
          column++;    
        } // else if
        else if ( mlines[row].ch[column] == '^' ) {                                     // ^
          temp.push_back( mlines[row].ch[column] ) ;
          token.token = token.token + temp ;
          token.type = XOR ;
          token.row = row ;
          msToken.push_back( token ) ;
          column++;    
        } // else if
        else if ( mlines[row].ch[column] == EOF ) {
          quit = true ;
        } // else if
        else {
          temp.push_back( mlines[row].ch[column] );
          token.token = token.token + temp ;
          token.type =  Unrecognize;
          token.row = row ;
          msToken.push_back( token ) ;
          column++;      
        } // else
      } // if
      else if ( mlines[row].ch[column] == '\n' ) {   
        row++ ;
        column = 0 ;
      } // else if
      else {
        column++;    
      } // else
                
      temp = "" ;
      token.token = "" ;
      token.type = 0;
          
    } // while 
                  
  } // Scanner()
       
  
  void Parsar() {
      
    mNotDef = false ;
    mQuit = false ;
    mtokenNum = 0 ;
    bool user_inputCorrect = false ;
    Formula user_inputValue ;
    bool than = false ;
    int row = 0;
    int num = 0;
    bool mistake = false ;
    bool live = false ;
    bool quit = false ;
    int tokenType = 0 ;
    string tokenValue ;
        
    cout << "Our-C running ..." << endl ;
      
          
    while ( mtokenNum < msToken.size() && mQuit == false )  {
                            
      User_input( user_inputCorrect, user_inputValue, quit )  ;
        
      if ( quit == false ) {
        Print() ;
      } // if 
      else {
        mQuit = true ;
      } // else
        
      if ( msToken[mtokenNum].type == eof )  mQuit = true ;
      
      quit = false ;
      mNotDef = false ;         
      mNotDEF.clear() ;
      mError.insStart = 0 ; 
       
          
    } // while()
    
             
    cout << "Our-C exited ..." ;      
      
      
  } // Parsar()
  
  void SaveFun( int num, string name, bool &live ) {
      
    Fun temp ;
    temp.name = name ;
      
    for ( int i = num ; i < mtokenNum ; i++ )             // 將function記錄下來 
      temp.content.push_back( msToken[i] ) ;
    
    for ( int i = 0 ; i < mFunDef.size() ; i++ ) {
      
      if ( mFunDef[i].name == name ) {
        mFunDef[i].content = temp.content ;
        live = true ;
      } // if
      
    }  // for
    
    
    if ( live == false ) mFunDef.push_back( temp ) ;
       
      
  } // SaveFun()
  
  void SaveVariable( string type, string name, string value, bool &live, string size, bool haveSize ) {
      
    Var temp ;
    int tokenType = 0 ;
    bool first = true ;
    int num = atoi( size.c_str() ) ;
    string strTemp = "" ;
    live = false ;
    
    if ( type == "int" ) {
      tokenType = INT ;
    }  // if
    else if ( type == "char" ) {
      tokenType = CHAR ;
    } // else if
    else if ( type == "float" ) {
      tokenType = FLOAT ;
    } // else if
    else if ( type == "string" ) {
      tokenType = STRING ;
    } // else if
    else if ( type == "bool" ) {
      tokenType = BOOL ;
    } // else if
      
    for ( int i = 0 ; i < mVarDef.size() ; i++ ) {
      if ( mVarDef[i].name == name ) {
        mVarDef[i].type = tokenType ;
        mVarDef[i].value = value ;
        mVarDef[i].first = false ;
        live = true ;
        if ( haveSize == true ) 
          mVarDef[i].size = size ;
          
        mVarDef[i].array.clear() ;
        
        for ( int j = 0 ; j < num ; j++ )        //  創造陣列儲存空間 
          mVarDef[i].array.push_back( strTemp ) ; 
      } // if 
    }  // for
    
    
    if ( live == false ) {
      temp.type = tokenType ;
      temp.name = name ;
      temp.value = value ;
      temp.first = true ;
      if ( haveSize == true )  {
        temp.size = size ;
      } // if
      
      temp.haveSize = haveSize ;
      
      for ( int j = 0 ; j < num ; j++ )        //  創造陣列儲存空間 
        temp.array.push_back( strTemp ) ;
      mVarDef.push_back( temp ) ;
    }  // if
    
    
    
    for ( int i = 0 ; i < mVarTemp.size() ; i++ ) {
      for ( int j = 0 ; j < mVarDef.size() ; j++ ) {

        if ( mVarDef[j].name ==  mVarTemp[i].name ) {
          mVarDef[j].type = tokenType ;
          mVarDef[j].value = value ;
          mVarDef[j].first = false ;
          if ( mVarTemp[i].haveSize == true ) 
            mVarDef[i].size = size ;

          first = false ;
          mVarTemp[i].first = false ;
          
          for ( int j = 0 ; j < num ; j++ )        //  創造陣列儲存空間 
            mVarDef[i].array.push_back( strTemp ) ;
          
        } // if
      } // for
      
      if ( first == true ) {
        temp.type = tokenType ;
        temp.name = mVarTemp[i].name;
        temp.value = value ;
        temp.first = true ;
        if ( mVarTemp[i].haveSize == true ) 
          temp.size = size ;

        temp.haveSize = mVarTemp[i].haveSize ;

        for ( int j = 0 ; j < num ; j++ )        //  創造陣列儲存空間 
          temp.array.push_back( strTemp ) ;

        mVarDef.push_back( temp ) ;
        mVarTemp[i].first = true ;
      }  // if
      
    }  // for
    
  } // SaveVariable()
  
  void SaveFunVar( string type, string name, string value  ) {
      
    Var temp ;
    int tokenType = 0;
    bool live = false ;
    bool first = true ;  
      
    if ( type == "int" ) {
      tokenType = INT ;
    }  // if
    else if ( type == "char" ) {
      tokenType = CHAR ;
    } // else if
    else if ( type == "float" ) {
      tokenType = FLOAT ;
    } // else if
    else if ( type == "string" ) {
      tokenType = STRING ;
    } // else if
    else if ( type == "bool" ) {
      tokenType = BOOL ;
    } // else if
      
    for ( int i = 0 ; i < mFunVarDef.size() ; i++ ) {
      if ( mFunVarDef[i].name == name ) {
        mFunVarDef[i].type = tokenType ;
        mFunVarDef[i].value = value ;
        live = true ;
      } // if
      
    }  // for
    
    
    if ( live == false ) {
      temp.type = tokenType ;
      temp.name = name ;
      temp.value = value ;
      mFunVarDef.push_back( temp ) ;
    }  // if
    
    for ( int i = 0 ; i < mVarTemp.size() ; i++ ) {
      for ( int j = 0 ; j < mFunVarDef.size() ; j++ ) {

        if ( mFunVarDef[j].name ==  mVarTemp[i].name ) {
          mFunVarDef[j].type = tokenType ;
          mFunVarDef[j].value = value ;
          mFunVarDef[j].first = false ;
          first = false ;
        } // if
      } // for
      
      if ( first == true ) {
        temp.type = tokenType ;
        temp.name = mVarTemp[i].name;
        temp.value = value ;
        temp.first = true ;
        mFunVarDef.push_back( temp ) ;
      }  // if
     
      first = true ;
    } // for  
  } // SaveFunVar()

  void Compute( Formula &value1, Formula value2, int type ) {
    int tempInt1 = 0;
    int tempInt2 = 0;
    double tempFloat1 = 0.0 ;
    double tempFloat2 = 0.0;
    char tempChar1  ;
    char tempChar2 ;
    bool tempBool1 = false ;
    bool tempBool2 = false ;
    stringstream sstream;
    
    
    if ( value1.type == INT ) {                             // INT 
      if ( value2.type == INT ) {

        tempInt1 = atoi( value1.value.c_str() ) ;        // 轉成int 
        tempInt2 = atoi( value2.value.c_str() ) ;        // 轉成int  

        if ( type == PLUS ) {                  // '+'
          tempInt1 = tempInt1 + tempInt2 ; 
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // if
        else if ( type == SUB ) {              // '-'
          tempInt1 = tempInt1 - tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if 
        else if ( type == MULT ) {             // '*'
          tempInt1 = tempInt1 * tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
          
        } // else if
        else if ( type == DIV ) {              // '/'
          tempInt1 = tempInt1 / tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == remainder ) {        // '%'
          tempInt1 = tempInt1 % tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == RS ) {                   // '>>'
          tempInt1 = tempInt1 >> tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == LS ) {                   // '<<'
          tempInt1 = tempInt1 << tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == PE ) {                   // '+='
          tempInt1 = tempInt1 + tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == ME ) {                   // '-='
          tempInt1 = tempInt1 - tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == TE ) {                   // '*='
          tempInt1 = tempInt1 * tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == DE ) {                   // '/='
          tempInt1 = tempInt1 / tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == RE ) {                   // '%='
          tempInt1 = tempInt1 % tempInt2 ;
          sstream << tempInt1 ;
          value1.value = sstream.str() ;
          value1.type = INT ;
        } // else if
        else if ( type == GT ) {               // '>'

          if ( tempInt1 > tempInt2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;
          
        } // else if
        else if ( type == LT ) {              // '<'
        
          if ( tempInt1 < tempInt2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;
          
        } // else if
        else if ( type == GE ) {              // '>='
        
          if ( tempInt1 >= tempInt2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == LE ) {              // '<='
        
          if ( tempInt1 <= tempInt2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == EQ ) {              // '=='
        
          if ( tempInt1 == tempInt2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == NEQ ) {             // '!='
        
          if ( tempInt1 != tempInt2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if	
        
        
      } // if
      else if ( value2.type == FLOAT ) {

        tempFloat1 = atof( value1.value.c_str() ) ;        // 轉成float 
        tempFloat2 = atof( value2.value.c_str() ) ;        // 轉成float  

        if ( type == PLUS ) {                      // '+'
          tempFloat1 = tempFloat1 + tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // if
        else if ( type == SUB ) {                  // '-'
          tempFloat1 = tempFloat1 - tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if 
        else if ( type == MULT ) {                 // '*'
          tempFloat1 = tempFloat1 * tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == DIV ) {                  // '/'
          tempFloat1 = tempFloat1 / tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == PE ) {                   // '+='
          tempFloat1 = tempFloat1 + tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == ME ) {                   // '-='
          tempFloat1 = tempFloat1 - tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == TE ) {                   // '*='
          tempFloat1 = tempFloat1 * tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == DE ) {                   // '/='
          tempFloat1 = tempFloat1 / tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == GT ) {               // '>'

          if ( tempFloat1 > tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;
          
        } // else if
        else if ( type == LT ) {              // '<'
        
          if ( tempFloat1 < tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;
          
        } // else if
        else if ( type == GE ) {              // '>='
        
          if ( tempFloat1 >= tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == LE ) {              // '<='
        
          if ( tempFloat1 <= tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == EQ ) {              // '=='
        
          if ( tempFloat1 == tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == NEQ ) {             // '!='
        
          if ( tempFloat1 != tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if

      } // else if
      else if ( value2.type == STRING ) {
        if ( type == PLUS ) {           // '+'
      

        } // if
        else if ( type == SUB ) {         // '-'

        } // else if 
        else if ( type == MULT ) {         // '*'

        } // else if
        else if ( type == DIV ) {           // '/'

        } // else if
        else if ( type == remainder ) {     // '%'

        } // else if	
        
        value1.type = STRING ;
        value1.value = value2.value ;
      } // else if 

    } // if
    else if  ( value1.type == FLOAT ) {                     // FLOAT

      if ( value2.type == INT ) {

        tempFloat1 = atof( value1.value.c_str() ) ;        // 轉成float 
        tempFloat2 = atof( value2.value.c_str() ) ;        // 轉成float  

        if ( type == PLUS ) {                      // '+'
          tempFloat1 = tempFloat1 + tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // if
        else if ( type == SUB ) {                  // '-'
          tempFloat1 = tempFloat1 - tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if 
        else if ( type == MULT ) {                 // '*'
          tempFloat1 = tempFloat1 * tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == DIV ) {                  // '/'
          tempFloat1 = tempFloat1 / tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == PE ) {                   // '+='
          tempFloat1 = tempFloat1 + tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == ME ) {                   // '-='
          tempFloat1 = tempFloat1 - tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == TE ) {                   // '*='
          tempFloat1 = tempFloat1 * tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == DE ) {                   // '/='
          tempFloat1 = tempFloat1 / tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == GT ) {               // '>'

          if ( tempFloat1 > tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;
          
        } // else if
        else if ( type == LT ) {              // '<'
        
          if ( tempFloat1 < tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;
          
        } // else if
        else if ( type == GE ) {              // '>='
        
          if ( tempFloat1 >= tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == LE ) {              // '<='
        
          if ( tempFloat1 <= tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == EQ ) {              // '=='
        
          if ( tempFloat1 == tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == NEQ ) {             // '!='
        
          if ( tempFloat1 != tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        
      } // if
      else if ( value2.type == FLOAT ) {

        tempFloat1 = atof( value1.value.c_str() ) ;        // 轉成float 
        tempFloat2 = atof( value2.value.c_str() ) ;        // 轉成float  

        if ( type == PLUS ) {                      // '+'
          tempFloat1 = tempFloat1 + tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // if
        else if ( type == SUB ) {                  // '-'
          tempFloat1 = tempFloat1 - tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if 
        else if ( type == MULT ) {                 // '*'
          tempFloat1 = tempFloat1 * tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == DIV ) {                  // '/'
          tempFloat1 = tempFloat1 / tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == PE ) {                   // '+='
          tempFloat1 = tempFloat1 + tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == ME ) {                   // '-='
          tempFloat1 = tempFloat1 - tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == TE ) {                   // '*='
          tempFloat1 = tempFloat1 * tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == DE ) {                   // '/='
          tempFloat1 = tempFloat1 / tempFloat2 ;
          sstream << tempFloat1 ;
          value1.value = sstream.str() ;
          value1.type = FLOAT ;
        } // else if
        else if ( type == GT ) {               // '>'

          if ( tempFloat1 > tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;
          
        } // else if
        else if ( type == LT ) {              // '<'
        
          if ( tempFloat1 < tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;
          
        } // else if
        else if ( type == GE ) {              // '>='
        
          if ( tempFloat1 >= tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == LE ) {              // '<='
        
          if ( tempFloat1 <= tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == EQ ) {              // '=='
        
          if ( tempFloat1 == tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if
        else if ( type == NEQ ) {             // '!='
        
          if ( tempFloat1 != tempFloat2 ) 
            tempBool1 = true ;
          else tempBool1 = false ;
          
          if ( tempBool1 == true ) 
            value1.value = "true" ;
          else value1.value = "false" ;
          
          value1.type = BOOL ;

        } // else if

      } // else if
      else if ( value2.type == STRING ) {
        if ( type == PLUS ) {           // '+'
      

        } // if
        else if ( type == SUB ) {         // '-'

        } // else if 
        
        value1.type = STRING ;
        value1.value = value2.value ;
        
      } // else if 
      else {                   // 型別無法計算 

      } // else

    } // else if
    else if  ( value1.type == STRING ) {

      if ( value2.type == INT ) {

      } // if
      else if ( value2.type == FLOAT ) {

      } // else if
      else if ( value2.type == STRING ) {

        if ( type == PLUS ) {           // '+'
          value1.value = value1.value + value2.value ;
        } // if
        else if ( type == PE ) {                   // '+='
          value1.value = value1.value + value2.value ;
        } // else if
        else if ( type == EQ ) {             // '=='
          if ( value1.value == value2.value ) 
            value1.value = "true" ;
          else value1.value = "false" ;

          value1.type = BOOL ; 
        } // if
        else if ( type == NEQ ) {       // '!='

          if ( value1.value != value2.value ) 
            value1.value = "true" ;
          else value1.value = "false" ;

          value1.type = BOOL ;

        } // else if

        value1.type = STRING ;
        
      } // else if 
      else if ( value2.type == CHAR ) {

        if ( type == PLUS ) {           // '+'
          value1.value = value1.value + value2.value ;
        } // if
        else if ( type == PE ) {                   // '+='
          value1.value = value1.value + value2.value ;
        } // else if
        else if ( type == EQ ) {             // '=='
          if ( value1.value == value2.value ) 
            value1.value = "true" ;
          else value1.value = "false" ;

          value1.type = BOOL ; 
        } // if
        else if ( type == NEQ ) {       // '!='

          if ( value1.value != value2.value ) 
            value1.value = "true" ;
          else value1.value = "false" ;

          value1.type = BOOL ;

        } // else if
        
        value1.type = STRING ;
      } // else if 
      
      else {                   // 型別無法計算 

      } // else

    } // else if
    else if  ( value1.type == CHAR ) {
      if ( value2.type == CHAR ) {

        if ( type == PLUS ) {           // '+'
          value1.value = value1.value + value2.value ;
        } // if
        else if ( type == PE ) {                   // '+='
          value1.value = value1.value + value2.value ;
        } // else if

        value1.type = STRING ;
      } // if
      else  if ( value2.type == STRING ) {
        if ( type == PLUS ) {           // '+'
          value1.value = value1.value + value2.value ;
        } // if
        else if ( type == PE ) {                   // '+='
          value1.value = value1.value + value2.value ;
        } // else if

        value1.type = STRING ;
      } // if
      else {                   // 型別無法計算 

      } // else

    } // else if
    else if  ( value1.type == BOOL ) {
      if ( value2.type == BOOL ) {
        if ( type == EQ ) {             // '=='
          if ( value1.value == value2.value ) 
            value1.value = "true" ;
          else value1.value = "false" ;

          value1.type = BOOL ; 
        } // if
        else if ( type == NEQ ) {       // '!='

          if ( value1.value != value2.value ) 
            value1.value = "true" ;
          else value1.value = "false" ;

          value1.type = BOOL ;

        } // else if
        else if ( type == AND ) {       // '&&'

          if ( value1.value == "true" && value2.value == "true" )  
            value1.value = "true" ;
          else value1.value = "false" ;

          value1.type = BOOL ;

        } // else if
        else if ( type == OR ) {       // '||'

          if ( value1.value == "true" || value2.value == "true" )  
            value1.value = "true" ;
          else value1.value = "false" ;

          value1.type = BOOL ;

        } // else if
        else {

        } // else 
      } // if
      else {                   // 型別無法計算 

      } // else

    } // else if
    else if ( value1.type == 0 ) {
      value1.type = value2.type ;
      value1.value = value2.value ;
    } // else if




  } // Compute()
  
  
  
  void OperateFun( bool &quit ) {
      
    int times = 0 ;
    string name ;
    Fun funTemp ;
    Var varTemp ;
    
      
    if ( mTempFun[0] == "ListAllFunctions" ) {           // 印出所有Fun
      for ( int i = 0 ; i < mFunDef.size() ; i++ ) {
        for ( int j = i ; j < mFunDef.size() ; j++ ) {
          if ( mFunDef[i].name > mFunDef[j].name ) {
            funTemp = mFunDef[i] ;
            mFunDef[i] = mFunDef[j] ;
            mFunDef[j] = funTemp ;
          } // if 
        } // for
      }  // for   
          
          
          
      for ( int i = 0 ; i < mFunDef.size() ; i++ ) 
        cout << mFunDef[i].name << "()" << endl ;       
 
    } // if
    else if ( mTempFun[0] == "ListFunction" ) {                // 印出指定Fun 
      
      
      for ( int num = 0 ; num < mTempVar.size() ; num++ ) {
        
        name = mTempVar[num] ;
       
        name.erase( 0, 1 ) ;
        
        name.erase( name.size()-1, 1 ) ;         // 將" 去掉 
            
        
        for ( int i = 0 ; i < mFunDef.size() ; i++ ) {
          
          if ( mFunDef[i].name == name ) {
              
            for ( int j = 0 ; j < mFunDef[i].content.size() ; j++ ) {
              
              if ( mFunDef[i].content[j].token == ";" ) {
                
                cout << mFunDef[i].content[j].token << endl ;    
                
                if ( mFunDef[i].content[j+1].token == "}" ) {
                  for ( int k = 0 ; k < times-2 ; k++ ) {
                    cout << " " ;
                  } // for
                } // if
                else  {
                  for ( int k = 0 ; k < times ; k++ ) {
                    cout << " " ;
                  } // for
                } // else
            
              } // if
              else  if ( mFunDef[i].content[j].token == "{" ) {
              
                times = times + 2 ;
                cout << mFunDef[i].content[j].token << endl ;
                for ( int k = 0 ; k < times ; k++ ) {
                  cout << " " ;
                } // for
            
              } // else if
              else  if ( mFunDef[i].content[j].token == "}" ) {
            
                times = times - 2 ;
                cout << mFunDef[i].content[j].token << endl ;
                if ( j + 1 < mFunDef[i].content.size() ) {
                  if ( mFunDef[i].content[j+1].token == "}" ) {
                    for ( int k = 0 ; k < times-2 ; k++ ) {
                      cout << " " ;
                    } // for
                  } // if
                  else  {
                    for ( int k = 0 ; k < times ; k++ ) {
                      cout << " " ;
                    } // for
                  } // else
                } // if
              } // else if
              else if ( mFunDef[i].content[j].type == Identifier &&
                        mFunDef[i].content[j+1].token == "(" )  
                cout << mFunDef[i].content[j].token ;
              else if ( mFunDef[i].content[j].type == Identifier &&
                        mFunDef[i].content[j+1].token == "++" ) 
                cout << mFunDef[i].content[j].token ;
              else if ( mFunDef[i].content[j].type == Identifier &&
                        mFunDef[i].content[j+1].token == "--" ) 
                cout << mFunDef[i].content[j].token ;
              else if ( mFunDef[i].content[j].type == Identifier &&
                        mFunDef[i].content[j+1].token == "[" ) 
                cout << mFunDef[i].content[j].token ;
              else if ( mFunDef[i].content[j].token == "]" &&
                        mFunDef[i].content[j+1].token == "," ) 
                cout << mFunDef[i].content[j].token ;
              else if ( mFunDef[i].content[j].type == Identifier &&
                        mFunDef[i].content[j+1].token == "," ) 
                cout << mFunDef[i].content[j].token ; 
              else cout << mFunDef[i].content[j].token << " " ;
       
            }  // for
          } // if
        } // for
      }  // for
        
        
    } // else if
    else if ( mTempFun[0] == "ListAllVariables" ) {           // 印出所有var 
    
      for ( int i = 0 ; i < mVarDef.size() ; i++ ) {
        for ( int j = i ; j < mVarDef.size() ; j++ ) {
          if ( mVarDef[i].name > mVarDef[j].name ) {
            varTemp = mVarDef[i] ;
            mVarDef[i] = mVarDef[j] ;
            mVarDef[j] = varTemp ;
          } // if 
        } // for
      }  // for
        
      for ( int i = 0 ; i < mVarDef.size() ; i++ ) {
        cout << mVarDef[i].name  << endl ;
      } // for
        
    } // else if
    else if ( mTempFun[0] == "ListVariable" ) {                // 印出指定var 

      for ( int num = 0 ; num < mTempVar.size() ; num++ ) {
         
        name = mTempVar[num] ;
        name.erase( 0, 1 ) ;
        name.erase( name.size()-1, 1 ) ;        // 將" 去掉 
            
        
        for ( int i = 0 ; i < mVarDef.size() ; i++ ) {
          
          if ( mVarDef[i].name  == name ) {

            if ( mVarDef[i].type == INT )          cout << "int " ;
            else if ( mVarDef[i].type == FLOAT )   cout << "float " ;
            else if ( mVarDef[i].type == CHAR )    cout << "char " ;
            else if ( mVarDef[i].type == STRING )  cout << "string " ;
            else if ( mVarDef[i].type == BOOL )    cout << "bool " ;
            
            cout << name ;
            if ( mVarDef[i].haveSize == true ) 
              cout << "[ " << mVarDef[i].size << " ]" ;
            
            cout << " ;" << endl ; 
            
          } // if
        } // for
      }  // for
        
        
    } // else if
    else if ( mTempFun[0] == "Done" ) {
      
      quit = true;
    } // else if
    else if ( mTempFun[0] == "cout" ) {
      
      for ( int i = 0 ; i < mOutput.size() ; i++ ) {

        for ( int j = 0 ; j < mOutput[i].value.size() ; j++ ) {

          if ( mOutput[i].value[j] == 92 && mOutput[i].value[j+1] == 'n' ) {
            cout << endl ;
            j++ ;
          } // if
          else cout << mOutput[i].value[j] ;

        } // for

      } // for
    } // else if
    
    
    
  } // OperateFun()
  
  bool CheckID( string token ) {

    if ( token == "ListAllFunctions" ) return true ;
    else if ( token == "ListFunction" ) return true ;
    else if ( token == "ListAllVariables" ) return true ;
    else if ( token == "ListVariable" ) return true ;
    else if ( token == "Done" ) return true ;
    else if ( token == "cin" ) return true ;
    else if ( token == "cout" ) return true ;
    
    for ( int i = 0 ; i < mFunVarDef.size() ; i++ ) {

      if ( mFunVarDef[i].name == token ) return true ;
      
    } // for

    for ( int i = 0 ; i < mVarDef.size() ; i++ ) {
      if ( mVarDef[i].name == token ) return true ;
    } // for
    
    for ( int i = 0 ; i < mFunDef.size() ; i++ ) {

      if ( mFunDef[i].name == token ) return true ;

    } // for
    
    return false ;

  } // CheckID()
  
  bool IsDigit( string str ) {         // 判斷是否數字 

    bool digit = true ;

    for ( int i = 0 ; i < str.size() ; i++ ) {
      if ( ( str[i] <= 48 || str[i] >= 57 ) && str[i] != 46 ) 
        return false ;
    } // for

    return true ;

  } // IsDigit()
  
  void SearchArray( string name, Formula size, Formula &value ) {

    int num = atoi( size.value.c_str() ) ;
    bool quit = false ;
    
    for ( int i = 0 ; i < mVarDef.size() && quit == false ; i++ ) {
      if ( mVarDef[i].name == name ) {
        if ( mVarDef[i].haveSize == true ) {
          value.value = mVarDef[i].array[num] ;   
          value.type =  mVarDef[i].type ;
        } // if
        else {
          value.value = mVarDef[i].value ;   
          value.type =  mVarDef[i].type ;
        } // else

        quit = true ;
      } // if
    }  // for

  } // SearchArray()
  
  void Assign_Var( string name, Formula size, Formula value ) {

    int num = atoi( size.value.c_str() ) ;
    bool quit = false ;
    
    for ( int i = 0 ; i < mVarDef.size() && quit == false ; i++ ) {
      if ( mVarDef[i].name == name ) {
        if ( mVarDef[i].haveSize == true ) {
          mVarDef[i].array[num] = value.value;   
          mVarDef[i].type = value.type;
        } // if
        else {
          mVarDef[i].value = value.value;   
          mVarDef[i].type = value.type;
        } // else

        quit = true ;
      } // if
    }  // for

  } // Assign_Var()
  
  
    
  
  void Print( )  
  {
    int line = 0;
    int row = 0 ;
    bool mistake = false ;
    
    
    if ( mError.insStart > 0 ) 
      line = msToken[mtokenNum].row - msToken[mError.insStart-1].row ;         //  safecode 可能 
    else line = msToken[mtokenNum].row - msToken[mError.insStart].row + 1 ;
        
        
    if ( mNotDef == true ) {
      cout << "> Line " << line << " : undefined identifier : '" ; 
      cout << msToken[mtokenNum].token << "'" << endl ;
    } // if 
    else {

      if ( msToken[mtokenNum].type ==  Unrecognize ) {
        cout << "> Line " << line << " : unrecognized token with first char : '" ;
        cout << msToken[mtokenNum].token << "'" << endl ;
      } // else if( type == Unrecognize )  
      else  {
        cout << "> Line " << line << " : unexpected token : '"  ;
        cout << msToken[mtokenNum].token << "'" << endl ;
      } // else
      
    }  // else
    
    row = msToken[mtokenNum].row ;      // 錯誤的行數
                    
    while ( mistake == false ) {
                        
      if ( msToken[mtokenNum].row != row ) 
        mistake = true ;
      else if ( ( mtokenNum+1 ) < msToken.size() ) {
        mtokenNum++ ;
      } // else if
                    
    } // while( mistake == false )
      
  } // Print()
  
  
  
  void PeekToken( int &tokenType, string &tokenValue ) {
      
      
    if ( msToken[mtokenNum].type == Identifier ) {
      tokenType = Identifier ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    }  // if
    else if ( msToken[mtokenNum].type == Constant ) {
      tokenType = Constant ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == INT ) {
      tokenType = INT ;
      tokenValue = msToken[mtokenNum].token ;
      return ;    
    } // else if
    else if ( msToken[mtokenNum].type == FLOAT ) {
      tokenType = FLOAT ;
      tokenValue = msToken[mtokenNum].token ;
      return ;    
    } // else if
    else if ( msToken[mtokenNum].type == CHAR ) {
      tokenType = CHAR ;
      tokenValue = msToken[mtokenNum].token ;
      return ;    
    } // else if
    else if ( msToken[mtokenNum].type == BOOL ) {
      tokenType = BOOL ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == STRING ) {
      tokenType = STRING ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == VOID ) {
      tokenType = VOID ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == IF ) {
      tokenType = IF ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == ELSE ) {
      tokenType = ELSE ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == WHILE ) {
      tokenType = WHILE ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == DO ) {
      tokenType = DO ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RETURN ) {
      tokenType = RETURN ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LP ) {                         // '('
      tokenType = LP ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RP ) {                         // ')'
      tokenType = RP ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LSB ) {                        // '['
      tokenType = LSB ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RSB ) {                        // ']'
      tokenType = RSB ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LCB ) {                        // '{'
      tokenType = LCB ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RCB ) {                        // '}'
      tokenType = RCB ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == PLUS ) {                       // '+' 
      tokenType = PLUS ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == SUB ) {                        // '-' 
      tokenType = SUB ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == MULT ) {                       // '*' 
      tokenType = MULT ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == DIV ) {                        // '/' 
      tokenType = DIV ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == remainder ) {                  // '%' 
      tokenType = remainder ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == XOR ) {                        // '^' 
      tokenType = XOR ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == GT ) {                         // '>' 
      tokenType = GT ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LT ) {                         // '<' 
      tokenType = LT ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == GE ) {                         // '>=' 
      tokenType = GE ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LE ) {                         // '<=' 
      tokenType = LE ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == EQ ) {                         // '==' 
      tokenType = EQ ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == NEQ ) {                        // '!=' 
      tokenType = NEQ ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == BAnd ) {                       // '&' 
      tokenType = BAnd ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == BOr ) {                        // '|' 
      tokenType = BOr ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == assignment ) {                 // '=' 
      tokenType = assignment ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == EM ) {                         // '!' 
      tokenType = EM ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == AND ) {                        // '&&' 
      tokenType = AND ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == OR ) {                         // '||' 
      tokenType = OR ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == PE ) {                         // '+=' 
      tokenType = PE ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == ME ) {                         // '-=' 
      tokenType = ME ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == TE ) {                         // '*=' 
      tokenType = TE ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == DE ) {                         // '/=' 
      tokenType = DE ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RE ) {                         // '%=' 
      tokenType = RE ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == PP ) {                         // '++' 
      tokenType = PP ;
      tokenValue = msToken[mtokenNum].token ;
      return ;    
    } // else if
    else if ( msToken[mtokenNum].type == MM ) {                         // '--' 
      tokenType = MM ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RS ) {                         // '>>' 
      tokenType = RS ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LS ) {                         // '<<' 
      tokenType = LS ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == SEMICOLON ) {                  // ';' 
      tokenType = SEMICOLON ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == COMMA ) {                      // ',' 
      tokenType = COMMA ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == QM ) {                         // '?' 
      tokenType = QM ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == COLON ) {                      // ':' 
      tokenType = COLON ;
      tokenValue = msToken[mtokenNum].token ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == Unrecognize ) {
      tokenType = Unrecognize ;
      tokenValue = msToken[mtokenNum].token ;
      return ;    
    } // else if
    else {
      tokenType = eof ;
      tokenValue = "" ;
      return ;        
    } // else
         
      
  } // PeekToken()  
  
  
  void GetToken( int &tokenType, string &tokenValue ) {
      
      
    if ( msToken[mtokenNum].type == Identifier ) {
      tokenType = Identifier ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    }  // if
    else if ( msToken[mtokenNum].type == Constant ) {
      tokenType = Constant ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == INT ) {
      tokenType = INT ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;    
    } // else if
    else if ( msToken[mtokenNum].type == FLOAT ) {
      tokenType = FLOAT ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;    
    } // else if
    else if ( msToken[mtokenNum].type == CHAR ) {
      tokenType = CHAR ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;    
    } // else if
    else if ( msToken[mtokenNum].type == BOOL ) {
      tokenType = BOOL ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == STRING ) {
      tokenType = STRING ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == VOID ) {
      tokenType = VOID ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == IF ) {
      tokenType = IF ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == ELSE ) {
      tokenType = ELSE ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == WHILE ) {
      tokenType = WHILE ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == DO ) {
      tokenType = DO ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RETURN ) {
      tokenType = RETURN ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LP ) {                         // '('
      tokenType = LP ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RP ) {                         // ')'
      tokenType = RP ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LSB ) {                        // '['
      tokenType = LSB ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RSB ) {                        // ']'
      tokenType = RSB ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LCB ) {                        // '{'
      tokenType = LCB ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RCB ) {                        // '}'
      tokenType = RCB ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == PLUS ) {                       // '+' 
      tokenType = PLUS ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == SUB ) {                        // '-' 
      tokenType = SUB ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == MULT ) {                       // '*' 
      tokenType = MULT ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == DIV ) {                        // '/' 
      tokenType = DIV ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == remainder ) {                  // '%' 
      tokenType = remainder ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == XOR ) {                        // '^' 
      tokenType = XOR ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == GT ) {                         // '>' 
      tokenType = GT ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LT ) {                         // '<' 
      tokenType = LT ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == GE ) {                         // '>=' 
      tokenType = GE ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LE ) {                         // '<=' 
      tokenType = LE ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == EQ ) {                         // '==' 
      tokenType = EQ ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == NEQ ) {                        // '!=' 
      tokenType = NEQ ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == BAnd ) {                       // '&' 
      tokenType = BAnd ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == BOr ) {                        // '|' 
      tokenType = BOr ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == assignment ) {                 // '=' 
      tokenType = assignment ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == EM ) {                         // '!' 
      tokenType = EM ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == AND ) {                        // '&&' 
      tokenType = AND ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == OR ) {                         // '||' 
      tokenType = OR ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == PE ) {                         // '+=' 
      tokenType = PE ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == ME ) {                         // '-=' 
      tokenType = ME ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == TE ) {                         // '*=' 
      tokenType = TE ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == DE ) {                         // '/=' 
      tokenType = DE ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RE ) {                         // '%=' 
      tokenType = RE ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == PP ) {                         // '++' 
      tokenType = PP ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;    
    } // else if
    else if ( msToken[mtokenNum].type == MM ) {                         // '--' 
      tokenType = MM ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == RS ) {                         // '>>' 
      tokenType = RS ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == LS ) {                         // '<<' 
      tokenType = LS ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == SEMICOLON ) {                  // ';' 
      tokenType = SEMICOLON ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == COMMA ) {                      // ',' 
      tokenType = COMMA ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == QM ) {                         // '?' 
      tokenType = QM ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    else if ( msToken[mtokenNum].type == COLON ) {                      // ':' 
      tokenType = COLON ;
      tokenValue = msToken[mtokenNum].token ;
      mtokenNum++ ;
      return ;
    } // else if
    
      
  } // GetToken()
         
  void User_input( bool &correct, Formula &value, bool &quit ) {                                           
  // user_input 
  // : ( definition | statement ) { definition | statement }
  
    int tokenType ;
    string tokenValue ;
    bool definitionCorrect = false;
    Formula definitionValue ;
    bool statementCorrect = false ;
    Formula statementValue ;
    string funName ;
    mFirst = false ;
    int num = mtokenNum ;
    Error temp ;
    temp.insStart = num ;
    quit = false ;
    bool live = false ;
    vector<Var> tempVar ;
    vector<Var> periodVar ;
    mTempVar.clear() ;
    mFunVarDef.clear() ;
    mTempFun.clear() ;
    mVarTemp.clear() ;
    bool fun = false ;
    periodVar = mVarDef ;
    mOutput.clear() ;
    PeekToken( tokenType, tokenValue ) ;      // 檢查是不是Def開頭 

    if ( tokenType == VOID || tokenType == INT || tokenType == FLOAT
         || tokenType == CHAR || tokenType == STRING || tokenType == BOOL ) {

      Definition( definitionCorrect, definitionValue, live, fun, funName ) ; 
      // 是不是 Definition  ,是的話傳回變數名或fun名

      if ( definitionCorrect == true ) {                          // 第一個 Definition存在 


        if ( live == false ) 
          cout << "> Definition of " << funName << " entered ..." << endl ; 
        else cout << "> New definition of " << funName << " entered ..." << endl ;
        if ( fun == false ) {
          for ( int i = 0 ; i < mVarTemp.size() ; i++ ) {
            if ( mVarTemp[i].first == true ) 
              cout << "Definition of " << mVarTemp[i].name << " entered ..." << endl ; 
            else cout << "New definition of " << mVarTemp[i].name << " entered ..." << endl ;
          } // for
        } // if

        do {

          PeekToken( tokenType, tokenValue ) ; // 檢查是不是Def開頭  
          live = false ;
          mTempFun.clear( ) ;                   //  要執行的FUN 
          mVarTemp.clear() ;                     // 要定義的Var 
          mFunVarDef.clear() ;
          mTempVar.clear() ; 
          num = mtokenNum ;
          temp.insStart = num ;                                // 指令開頭 
          fun = false ;
          periodVar = mVarDef ;
          mOutput.clear() ;
          
          
          if ( tokenType == VOID || tokenType == INT || tokenType == FLOAT
               || tokenType == CHAR || tokenType == STRING || tokenType == BOOL ) {
           
            Definition( definitionCorrect, definitionValue, live, fun, funName ) ; 
            // 是不是 Definition  ,是的話傳回變數名或fun名
        
            if ( definitionCorrect == false ) {
              temp.errorLine = mtokenNum ;
              mError = temp ;
              correct = true ;
              return ; 
            } // if
            else {
              if ( live == false ) 
                cout << "> Definition of " << funName << " entered ..." << endl ; 
              else cout << "> New definition of " << funName << " entered ..." << endl ;
              
              if ( fun == false ) {
                for ( int i = 0 ; i < mVarTemp.size() ; i++ ) {
                  if ( mVarTemp[i].first == true ) 
                    cout << "Definition of " << mVarTemp[i].name << " entered ..." << endl ; 
                  else cout << "New definition of " << mVarTemp[i].name << " entered ..." << endl ;
                } // for
              } // if

            } // else

          }  // if
          else {

            Statement( statementCorrect, statementValue ) ;      // 是不是 Statement
                
            if ( statementCorrect == false ) {                   // 都不是，但已經至少一個 ，回傳 true 
              temp.errorLine = mtokenNum ;
              mError = temp ;
              correct = true ;
              mVarDef = periodVar;
              return ;    
            } // if 
            else {
              cout << "> " ;
            
              if ( mTempFun.size() > 0 ) {
                OperateFun( quit ) ;    
              } // if
            
              if ( quit == true ) {
                correct = true ;
                return ;
              } // if
              else  cout << "Statement executed ..." << endl ;
              
            } // else
          }  // else
              
        } while ( true )    ;
        
      } // if
        
    } // if
    else {

      Statement( statementCorrect, statementValue ) ;      // 是不是 Statement

      if ( statementCorrect == true ) {                         // 第一個 Statement存在 

        cout << "> " ;
        if ( mTempFun.size() > 0 ) {
          OperateFun( quit ) ;    
        } // if
        
        if ( quit == true ) {
          correct = true ;
          return ;
        } // if
        else cout << "Statement executed ..." << endl ;
        
        do {

          PeekToken( tokenType, tokenValue ) ; // 檢查是不是Def開頭  
          mTempVar.clear() ;
          live = false ;
          mTempFun.clear( ) ;
          mFunVarDef.clear() ;
          mVarTemp.clear() ;
          num = mtokenNum ;
          temp.insStart = num ;                                // 指令開頭 
          fun = false ;
          periodVar = mVarDef ;
          mOutput.clear() ;
          
          if ( tokenType == VOID || tokenType == INT || tokenType == FLOAT
               || tokenType == CHAR || tokenType == STRING || tokenType == BOOL ) { 
           
            Definition( definitionCorrect, definitionValue, live, fun, funName ) ; 
            // 是不是 Definition  ,是的話傳回變數名或fun名
        
            if ( definitionCorrect == false ) {
              temp.errorLine = mtokenNum ;
              mError = temp ;
              correct = true ;
              return ; 
            } // if
            else {

              if ( live == false ) 
                cout << "> Definition of " << funName << " entered ..." << endl ;
              else cout << "> New definition of " << funName << " entered ..." << endl ;
              
              if ( fun == false ) {
                for ( int i = 0 ; i < mVarTemp.size() ; i++ ) {
                  if ( mVarTemp[i].first == true ) 
                    cout << "Definition of " << mVarTemp[i].name << " entered ..." << endl ; 
                  else cout << "New definition of " << mVarTemp[i].name << " entered ..." << endl ;
                } // for
              } // if

            } // else

          }  // if
          else {

            Statement( statementCorrect, statementValue ) ;      // 是不是 Statement
                
            if ( statementCorrect == false ) {                   // 都不是，但已經至少一個 ，回傳 true 
              temp.errorLine = mtokenNum ;
              mError = temp ;
              correct = true ;
              mVarDef = periodVar;
              return ;    
            } // if 
            else {
              cout << "> " ;
            
              if ( mTempFun.size() > 0 ) {
                OperateFun( quit ) ;    
              } // if
            
              if ( quit == true ) {
                correct = true ;
                return ;
              } // if
              else  cout << "Statement executed ..." << endl ;
              
            } // else
          }  // else
              
        } while ( true )    ;
         
      } // if
      else {
        temp.errorLine = mtokenNum ;
        mError = temp ;
        correct = false; 
        return;    
      } // else 

    } // else

    
    
    return ; 
     
  } // User_input()
  
  void Definition( bool &correct, Formula &value, bool &live, bool &fun, string &funName ) {
  //  definition 
  //  : VOID Identifier function_definition_without_ID 
  //    | type_specifier Identifier function_definition_or_declarators
    
    string tokenValue ;
    int tokenType = 0;
    bool type_specifierCorrect = false ;
    string type_specifierValue ;
    bool function_definition_without_IDCorrect = false ;
    Formula function_definition_without_IDValue ;
    bool function_definition_or_declaratorsCorrect = false ;
    Formula function_definition_or_declaratorsValue ;
    int num = mtokenNum ;
    int times ;
    string fName ;
    string number = "" ;
    string size ;
    bool haveSize = false ;
    fun = false ;
    bool have = false ;
    live = false;
   
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == VOID ) {     
    
      GetToken(  tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ; 
      
      if ( tokenType == Identifier ) {
          
        GetToken( tokenType, tokenValue ) ;
          
        fName = tokenValue ;            //   將名字紀錄 
          
        Function_definition_without_ID( function_definition_without_IDCorrect,               
                                        function_definition_without_IDValue ) ;
        // Function_definition_without_ID
        if ( function_definition_without_IDCorrect == false ) {
          correct = false ; 
          return ;
        } // if
        else {
          correct = true ;     
          SaveFun( num, fName, live ) ;                                     // num 是function 起始位置 
          funName = fName + "()" ;
          fun = true ;    
          return ;
        } // else
          
      } // if
      else {                                                              // tokenType不是 Identifier 
        correct = false ; 
        return ;
      } // else 
      
    } // if
    else {
                      
      Type_specifier( type_specifierCorrect, type_specifierValue ) ;
      
      if ( type_specifierCorrect == false ) {   
        correct = false ; 
        return ; 
      } // if
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == Identifier ) {
        
        
        GetToken( tokenType, tokenValue ) ;
          
        fName = tokenValue ;            //   將名字紀錄 
          
        Function_definition_or_declarators( function_definition_or_declaratorsCorrect,        
                                            function_definition_or_declaratorsValue, fun,
                                            size, haveSize ) ;
        // Function_definition_or_declarators     

        if ( function_definition_or_declaratorsCorrect == false ) {
          correct = false ; 
          return ;     
        } // if
        else {
          correct = true ;     
          if ( fun == true ) {
            SaveFun( num, fName, live ) ;
            fName = fName + "()" ;
          } // if
          else SaveVariable( type_specifierValue, fName, number, live, size, haveSize ) ;  // 第一次傳0進去
          
          funName = fName ;
          return ;
        
        } // else
      }  // if
      else {                  // tokenType不是 Identifier
        correct = false ; 
        return ;
      } // else
          
    } // else
      
  
  } // Definition()
  
  void Type_specifier( bool &correct, string &value ) {
  // type_specifier 
  // : INT | CHAR | FLOAT | STRING | BOOL
  
    int tokenType ;
    string tokenValue ;
    int num = mtokenNum ;

    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == INT ) {
      GetToken( tokenType, tokenValue ) ;    
      correct = true ;
      value = tokenValue ;
      return ;     
    }  // if
    else if ( tokenType == CHAR ) {
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      value = tokenValue ;
      return ;
    } // else if
    else if ( tokenType == FLOAT ) {
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      value = tokenValue ;
      return ;
    } // else if
    else if ( tokenType == STRING ) {
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      value = tokenValue ;
      return ;
    } // else if
    else if ( tokenType == BOOL ) {
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      value = tokenValue ;
      return ;
    } // else if
    else {
      correct = false ;
      value = 0.0 ;
      return ;    
    } // else
    
    
  } // Type_specifier() 
  
  void Function_definition_or_declarators( bool &correct, Formula &value, bool &fun, string &size, 
                                           bool &haveSize ) {
  // function_definition_or_declarators 
  // : function_definition_without_ID
  //   | rest_of_declarators
  
    bool function_definition_without_IDCorrect = false ;
    Formula function_definition_without_IDValue ;
    bool rest_of_declaratorsCorrect = false ;
    Formula rest_of_declaratorsValue ;
    int num = mtokenNum ;
    haveSize = false ;
    
    
    Function_definition_without_ID( function_definition_without_IDCorrect, 
                                    function_definition_without_IDValue ) ;

    if ( function_definition_without_IDCorrect == false ) {
        
      Rest_of_declarators( rest_of_declaratorsCorrect, rest_of_declaratorsValue, size, haveSize ) ;
      
      if ( rest_of_declaratorsCorrect == false ) {
        correct = false ;
        return ;
      } // if
      else {
        correct = true ;
        fun = false ;
        return ;    
      } // else
      
    } // if
    else {
      correct = true ;
      fun = true ;
      return ;    
    } // else 
      
  
  } // Function_definition_or_declarators() 
  
  void Rest_of_declarators( bool &correct, Formula &value, string &size, bool &haveSize ) {
  // rest_of_declarators 
  // : [ '[' Constant ']' ]  { ',' Identifier [ '[' Constant ']' ] } ';'
  
    int tokenType ;
    string tokenValue ;
    bool repeat = true ;
    int num = mtokenNum ;
    Var temp  ; 
    haveSize = false ;
    temp.haveSize = false ;
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == LSB ) {                               //  '['
    
      GetToken( tokenType, tokenValue ) ;
      
      PeekToken( tokenType, tokenValue ) ;
        
      if ( tokenType != Constant ) {
        correct = false ;
        return ;    
      } // if
      
      size = tokenValue ;
      haveSize = true ; 
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != RSB ) {                         //  NO ']'                     
        correct = false ;
        return ;    
      } // if
      
      GetToken( tokenType, tokenValue ) ;

    } // if
          
      
    do {

      temp.haveSize = false  ;
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != COMMA ) repeat = false ;      // NO ','
      else {
            
        GetToken( tokenType, tokenValue ) ;
        PeekToken( tokenType, tokenValue ) ;
        
        if ( tokenType != Identifier ) {                     // NO Identifier
          correct = false ;
          return ;        
        } // if
          
        temp.name = tokenValue ;
          
        GetToken( tokenType, tokenValue ) ;
        PeekToken( tokenType, tokenValue ) ;
    
        if ( tokenType == LSB ) {                               // '['
              
          GetToken( tokenType, tokenValue ) ;
          PeekToken( tokenType, tokenValue ) ;
            
          if ( tokenType != Constant ) {                         // NO Constant
            correct = false ;
            return ;    
          } // if
            
          temp.size = tokenValue ;
          temp.haveSize = true ;
          GetToken( tokenType, tokenValue ) ;
          PeekToken( tokenType, tokenValue ) ;
      
          if ( tokenType != RSB ) {                         //  NO ']'                     
            correct = false ;
            return ;    
          } // if
      
          GetToken( tokenType, tokenValue ) ;
            
        } // if
        
        mVarTemp.push_back( temp ) ;  
      } // else
        
    } while ( repeat == true ) ;
        
    PeekToken( tokenType, tokenValue ) ;
        
    if ( tokenType == SEMICOLON ) {                           // ';'
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      return ;
    } // if
    else {
      correct = false ;
      return ;
    } // else
      
    
  
  } // Rest_of_declarators()
  
  
  void Function_definition_without_ID( bool &correct, Formula &value ) {
  // function_definition_without_ID 
  // : '(' [ VOID | formal_parameter_list ] ')' compound_statement
    
    int tokenType ;
    string tokenValue ;
    bool compound_statementCorrect = false ;
    Formula compound_statementValue ;
    bool formal_parameter_listCorrect = false ;
    Formula formal_parameter_listValue ;
    int num = mtokenNum ;
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType != LP )  {                        // NO '('
      correct = false ;
      return ;        
    } // if
    
    GetToken( tokenType, tokenValue ) ;
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == VOID ) {
        
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;    
      
      if ( tokenType != RP ) {                         // NO ')'
        correct = false ;
        return ;    
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Compound_statement( compound_statementCorrect, compound_statementValue ) ;
      
      if ( compound_statementCorrect == false ) {
        correct = false ;
        return ;
      } // if
      else {
        correct = true ;
        return ;
      } // else
      
    } // if
    else {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == INT  || tokenType == FLOAT || tokenType == CHAR 
           || tokenType == STRING || tokenType == BOOL ) {
        Formal_parameter_list( formal_parameter_listCorrect, formal_parameter_listValue ) ;
      
        if ( formal_parameter_listCorrect == false ) {                         
          correct = false ;
          return ;    
        } // if
      }  // if
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != RP ) {                         // NO ')'
        correct = false ;
        return ;    
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Compound_statement( compound_statementCorrect, compound_statementValue ) ;
      
      if ( compound_statementCorrect == false ) {
        correct = false ;
        return ;
      } // if
      else {
        correct = true ;
        return ;
      } // else
      
    } // else
    
  } // Function_definition_without_ID()
  
  void Formal_parameter_list( bool &correct, Formula &value ) {
  // formal_parameter_list 
  // : type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
  //   { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
  
    int tokenType ;
    string tokenValue ;
    bool type_specifierCorrect = false ;
    string type_specifierValue ; 
    int num = mtokenNum ;
    bool live = false ;
    string number ;
    string size ;
    bool haveSize = false ;

    Type_specifier( type_specifierCorrect, type_specifierValue ) ;
   
    if ( type_specifierCorrect == false ) {
      correct = false ;
      return ; 
    } // if
   
    PeekToken( tokenType, tokenValue ) ;
   
    if ( tokenType == BAnd )                                             // "&"
      GetToken( tokenType, tokenValue ) ;
    
    PeekToken( tokenType, tokenValue ) ;
     
    if ( tokenType != Identifier ) { 
      correct = false ;
      return ;
    } // if
    
    SaveFunVar( type_specifierValue, tokenValue, number ) ;
    GetToken( tokenType, tokenValue ) ;
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == LSB ) {                       // '['
      
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != Constant ) { 
        correct = false ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != RSB ) {                   // ']' 
        correct = false ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
    
    } // if
    
    
    do { 
    
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != COMMA ) {                                       // ','
        correct = true ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Type_specifier( type_specifierCorrect, type_specifierValue ) ;
      
      if ( type_specifierCorrect == false ) {
        correct = false ;
        return ; 
      } // if
      
      PeekToken( tokenType, tokenValue ) ;
   
      if ( tokenType == BAnd )                                            // "&"
        GetToken( tokenType, tokenValue ) ;
    
      PeekToken( tokenType, tokenValue ) ;
     
      if ( tokenType != Identifier ) { 
        correct = false ;
        return ;
      } // if
      
      SaveFunVar( type_specifierValue, tokenValue, number ) ;
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;
    
      if ( tokenType == LSB ) {                          // '['
      
        GetToken( tokenType, tokenValue ) ;
        PeekToken( tokenType, tokenValue ) ;
      
        if ( tokenType != Constant ) { 
          correct = false ;
          return ;
        } // if
      
        GetToken( tokenType, tokenValue ) ;
        PeekToken( tokenType, tokenValue ) ;
      
        if ( tokenType != RSB ) {                     // ']' 
          correct = false ;
          return ;
        } // if
      
        GetToken( tokenType, tokenValue ) ;
    
      } // if
      
       
    } while ( true ) ;
    
  } // Formal_parameter_list()
  
  void Compound_statement( bool &correct, Formula &value ) { 
  // compound_statement 
  // : '{' { declaration | statement } '}'
  
    int tokenType ;
    string tokenValue ;
    bool declarationCorrect = false ;
    Formula declarationValue ; 
    bool statementCorrect = false ;
    Formula statementValue ;  
    bool repeat = true ; 
    int num = mtokenNum ;
    
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType != LCB ) {                       // '{' 
      correct = false ;
      return ;
    } // if 
    
    GetToken( tokenType, tokenValue ) ;
    
    
    do {

      PeekToken( tokenType, tokenValue ) ; // 檢查是不是Def開頭  
      

      if (  tokenType == INT || tokenType == FLOAT
           || tokenType == CHAR || tokenType == STRING || tokenType == BOOL ) { 

        
        Declaration( declarationCorrect, declarationValue ) ;
        
        if ( declarationCorrect == false ) {
          correct = false ;
          return ;
        }  // if
          
      }  // if
      else if ( tokenType ==  RCB )  repeat = false ;
      else {

        Statement( statementCorrect, statementValue ) ;

        if ( statementCorrect == false )  {
          correct = false ;
          return ;
        } // if

      } // else 


    } while ( repeat == true ) ; // do while()


    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType != RCB ) {                        // '}'
      correct = false ;
      return ;
    } // if
    
    GetToken( tokenType, tokenValue ) ;
    
    correct = true ;
    return ;
    
    
  
  } // Compound_statement()
  
  void Declaration( bool &correct, Formula &value ) {
  // declaration 
  // : type_specifier Identifier rest_of_declarators
  
    int tokenType = 0;
    string tokenValue ;
    bool type_specifierCorrect = false ;
    string type_specifierValue ; 
    bool rest_of_declaratorsCorrect = false ;
    Formula rest_of_declaratorsValue ;
    string number = "" ;
    bool live = false ;
    string size ;
    bool haveSize = false ; 
    
    Type_specifier( type_specifierCorrect, type_specifierValue ) ;
   
    if ( type_specifierCorrect == false ) {
      correct = false ;
      return ; 
    } // if
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType != Identifier ) { 
      correct = false ;
      return ; 
    } // if
    
    GetToken( tokenType, tokenValue ) ;
    
    Rest_of_declarators( rest_of_declaratorsCorrect, rest_of_declaratorsValue, size, haveSize ) ;
    
    if ( rest_of_declaratorsCorrect == false ) {
      correct = false ;
      return ; 
    } // if
    
    
    SaveFunVar( type_specifierValue, tokenValue, number ) ;
    
    correct = true ;
    return ;
  
  } // Declaration()
  
  void Statement( bool &correct, Formula &value ) {
  // statement
  // : ';' // the null statement
  //   | expression ';' /* expression here should not be empty */
  //   | RETURN [ expression ] ';'
  //   | compound_statement
  //   | IF '(' expression ')' statement [ ELSE statement ]
  //   | WHILE '(' expression ')' statement
  //   | DO statement WHILE '(' expression ')' ';'
  
    int tokenType ;
    string tokenValue ;
    bool compound_statementCorrect = false ;
    Formula compound_statementValue ;
    bool expressionCorrect = false ;
    Formula expressionValue ;
    bool statementCorrect = false ;
    Formula statementValue ;
    int num = 0;
    
    PeekToken( tokenType, tokenValue ) ;
    
    
    if ( tokenType == SEMICOLON ) {                          //  ;
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      return ;
    } // if
    
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == RETURN ) {             // RETURN
        
      GetToken( tokenType, tokenValue ) ;

      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == Identifier  || tokenType == PP || tokenType == MM 
           || tokenType == LP || tokenType == Constant || tokenType == PLUS 
           || tokenType == SUB || tokenType == EM ) {
        
        Expression( expressionCorrect, expressionValue ) ;
        
        if ( expressionCorrect == false ) {
          correct = false ;
          return ;
        } // if

      } // if    
      
          
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != SEMICOLON ) {                   // ';'
        correct = false ;
        return ;
      } // if
          
      GetToken( tokenType, tokenValue ) ;
          
      correct = true ;
      return ;
          
    } // if
    
    PeekToken( tokenType, tokenValue ) ;

    if ( tokenType == LCB ) {              // "{"
    
      Compound_statement( compound_statementCorrect, compound_statementValue ) ; 
    
      if ( compound_statementCorrect == false ) {
        correct = false ;
        return ;
      }  // if
      else {
        correct = true ;
        return ;
      } // else

    } // if

    PeekToken( tokenType, tokenValue ) ;

    if ( tokenType == IF ) {                // IF
        
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != LP ) {                  // '('
        correct = false ;
        return ;
      } // if
          
      GetToken( tokenType, tokenValue ) ;
          
      Expression( expressionCorrect, expressionValue ) ;
          
      if ( expressionCorrect == false ) { 
        correct = false ;
        return ;
      } // if
          
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != RP ) {                  // ')'
        correct = false ;
        return ;
      } // if
          
      GetToken( tokenType, tokenValue ) ;
          
      Statement( statementCorrect, statementValue ) ;
          
      if ( statementCorrect == false ) {
        correct = false ;
        return ;
      } // if
          
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != ELSE )  {           //  ELSE
        correct = true ;
        return ;
      }  // if
          
      GetToken( tokenType, tokenValue ) ;
          
      Statement( statementCorrect, statementValue ) ;
          
      if ( statementCorrect == false ) {
        correct = false ;
        return ;
      } // if
          
          
      correct = true ;
      return ;
          
    } // if
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == WHILE ) {             // WHILE
          
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != LP ) {                 // '('
        correct = false ;
        return ;
      } // if
          
      GetToken( tokenType, tokenValue ) ;
          
      Expression( expressionCorrect, expressionValue ) ;
          
      if ( expressionCorrect == false ) { 
        correct = false ;
        return ;
      } // if
          
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != RP ) {                 // ')'
        correct = false ;
        return ;
      } // if
          
      GetToken( tokenType, tokenValue ) ;
          
      Statement( statementCorrect, statementValue ) ;
              
      if ( statementCorrect == false ) {
        correct = false ;
        return ;
      } // if
          
      correct = true ;
      return ;
              
    } // if
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == DO ) {                 // DO
        
      GetToken( tokenType, tokenValue ) ;
          
      Statement( statementCorrect, statementValue ) ;
          
      if ( statementCorrect == false ) {
        correct = false ;
        return ;
      } // if
          
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != WHILE ) {
        correct = false ;
        return ;
      } // if
         
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != LP ) {              // '('
        correct = false ;
        return ;
      } // if
          
      GetToken( tokenType, tokenValue ) ;
          
      Expression( expressionCorrect, expressionValue ) ;
          
      if ( expressionCorrect == false ) { 
        correct = false ;
        return ;
      } // if
          
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != RP ) {              // ')'
        correct = false ;
        return ;
      } // if
          
      GetToken( tokenType, tokenValue ) ;
      PeekToken( tokenType, tokenValue ) ;
          
      if ( tokenType != SEMICOLON ) {         // ';'
        correct = false ;
        return ;
      } // if
          
      GetToken( tokenType, tokenValue ) ;
      correct = true ;                         // 都對回傳 
      return ;
              
    } // if
    
    Expression( expressionCorrect, expressionValue ) ;

    if ( expressionCorrect == true ) {

      PeekToken( tokenType, tokenValue ) ;

      if ( tokenType != SEMICOLON ) {                   // ';'
        correct = false ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      return ;
      
    } // if
    
    correct = false ;
    return ;

  
  
  
  } // Statement()
  
  void Expression( bool &correct, Formula &value ) {
  // expression
  // : basic_expression { ',' basic_expression }
  
    int tokenType = 0;
    string tokenValue ;
    bool basic_expressionCorrect = false ;
    Formula basic_expressionValue ;
    
    Basic_expression( basic_expressionCorrect, basic_expressionValue ) ;
    
    if ( basic_expressionCorrect == false ) {
      correct = false ;
      return ;    
    } // if
    
    
    do {
        
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != COMMA ) {          // NO ','
        correct = true ;
        value = basic_expressionValue ;
        return ;    
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Basic_expression( basic_expressionCorrect, basic_expressionValue ) ;
      
      if ( basic_expressionCorrect == false ) {
        correct = false ;
        return ;    
      } // if
      
    } while ( true ) ; // while()
    
  } // Expression()
  
  void Basic_expression( bool &correct, Formula &value ) {
  // basic_expression
  // : Identifier rest_of_Identifier_started_basic_exp
  //   | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  //   | sign { sign } signed_unary_exp romce_and_romloe
  //   | ( Constant | '(' expression ')' ) romce_and_romloe
    
    int tokenType = 0 ;
    string tokenValue ;
    bool signCorrect = false ;
    string signValue ;
    bool expressionCorrect = false ;
    Formula expressionValue ;
    bool romce_and_romloeCorrect = false ;
    Formula romce_and_romloeValue ;
    bool rest_of_Identifier_started_basic_expCorrect = false;
    Formula rest_of_Identifier_started_basic_expValue ;
    bool rest_of_PPMM_Identifier_started_basic_expCorrect = false ;
    Formula rest_of_PPMM_Identifier_started_basic_expValue ;
    bool signed_unary_expCorrect = false ;
    Formula signed_unary_expValue ;
    Formula temp ;
    int num = 0;
    bool repeat = true;
    bool isFloat = false ;
    double tempNUM ;
    int number = 0 ;
    int type = 0 ;
    stringstream sstream;
     
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == Identifier ) {  //
      
      if ( CheckID( tokenValue ) == false ) {
        mNotDef = true ;
        correct = false ;
        return ;
      }  // if
      
      
      GetToken( tokenType, tokenValue ) ;
      mTempFun.push_back( tokenValue ) ;         // 看是不是要執行的Fun 
      
      Rest_of_Identifier_started_basic_exp( rest_of_Identifier_started_basic_expCorrect,
                                            rest_of_Identifier_started_basic_expValue, tokenValue ) ;
      
                                                                              
      if ( rest_of_Identifier_started_basic_expCorrect == false ) {
        correct = false ;
        return ;
      }  // if 
      
      
      
      value = rest_of_Identifier_started_basic_expValue ;
      correct = true ;
      return ;
       
    
    } // if
    else if ( tokenType == PP || tokenType == MM ) {               // PP or MM
    
      GetToken( tokenType, tokenValue ) ;
      type = tokenType ;
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != Identifier ) { 
        correct = false ;
        return ;
      } // if
      
      if ( CheckID( tokenValue ) == false ) {
        mNotDef = true ;
        correct = false ;
        return ;
      }  // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Rest_of_PPMM_Identifier_started_basic_exp( rest_of_PPMM_Identifier_started_basic_expCorrect,
                                                 rest_of_PPMM_Identifier_started_basic_expValue, 
                                                 tokenValue, type ) ;
                                                 
      if ( rest_of_PPMM_Identifier_started_basic_expCorrect == false ) {
        correct = false ;
        return ;
      } // if
      
      value = rest_of_PPMM_Identifier_started_basic_expValue ;
      correct = true ;
      return ;
        
    } // else if
    else if ( tokenType == Constant || tokenType == LP ) {            // Constant or '('
    
      
      if ( tokenType == LP ) {                                   // '('
          
        GetToken( tokenType, tokenValue ) ;
        
        Expression( expressionCorrect, expressionValue ) ;
        
        if ( expressionCorrect == false ) {
          correct = false ;
          return ;    
        } // if 
        
        PeekToken( tokenType, tokenValue ) ;
        
        if ( tokenType != RP ) {                       // ')'
          correct = false ;
          return ;        
        } // if
        
        GetToken( tokenType, tokenValue ) ;
        
        temp = expressionValue ;
        
      } // if
      else {

        if ( tokenValue[0] == 34 ) {         // "
          tokenValue.erase( 0, 1 ) ;
          tokenValue.erase( tokenValue.size()-1, 1 ) ;         // 將" 去掉 
          temp.value = tokenValue ;
          temp.type = STRING ;
        }  // if
        else if ( tokenValue[0] == 39 ) {    // '
          tokenValue.erase( 0, 1 ) ;
          tokenValue.erase( 0, 1 ) ;
          tokenValue.erase( tokenValue.size()-1, 1 ) ;         // 將' 去掉 
          temp.value = tokenValue ;
          temp.type = CHAR ;
        } // else if
        else if ( tokenValue == "true" || tokenValue == "false" ) {
          temp.value = tokenValue ;
          temp.type = BOOL ;
        } // else if
        else {
          for ( int i = 0 ; i < tokenValue.size() ; i++ ) 
            if ( tokenValue[i] == '.' ) isFloat = true ;
        
          if ( isFloat == true && tokenValue[tokenValue.size()-1] == '.' ) {
            temp.value = tokenValue + "0" ; 
            temp.type = FLOAT ;
          } // if
          else if ( isFloat == true && tokenValue[0] == '.' ) {
            temp.value = "0" + tokenValue ; 
            temp.type = FLOAT ;
          } // else if
          else if ( isFloat == true && tokenValue[tokenValue.size()-1] != '.' && tokenValue[0] != '.' ) {
            temp.value = tokenValue ; 
            temp.type = FLOAT ;
          } // else if
          else {
            temp.value = tokenValue ;
            temp.type = INT ;
          } // else

        } // else

        GetToken( tokenType, tokenValue ) ;

        mTempVar.push_back( tokenValue ) ;             // 要印出的 Var ; 
        
      } // else
      
      romce_and_romloeValue = temp ;
      
      Romce_and_romloe( romce_and_romloeCorrect, romce_and_romloeValue ) ;
      
      if ( romce_and_romloeCorrect == false ) {
        correct = false ;
        return ;
      } // if
      
      correct = true ;
      value = romce_and_romloeValue ;
      return ;
       
    
    } // else if
    else {
        
      Sign( signCorrect, signValue ) ;
        
      if ( signCorrect == false ) {
        correct = false ;
        return ;
      } // if
      
      do {

        if ( signValue == "+" ) num++ ;
        else if ( signValue == "-" ) num-- ;  
        else if ( signValue == "!" ) number++ ;

        Sign( signCorrect, signValue ) ;
        
        if ( signCorrect == false ) repeat = false ;
         
      } while ( repeat == true ) ;
        
        
      Signed_unary_exp( signed_unary_expCorrect, signed_unary_expValue ) ;
      
      
      if ( signed_unary_expCorrect == false ) {
        correct = false ;
        return ;
      } // if
      
      if ( number % 2 != 0 ) {

        if ( signed_unary_expValue.value == "true" ) 
          signed_unary_expValue.value = "false" ;
        else if ( signed_unary_expValue.value == "false" ) {
          signed_unary_expValue.value = "true" ;
        } // else if

      } // if
      else if ( num >= 0 )  value = signed_unary_expValue ;
      else if ( num < 0 ) {
        tempNUM = atof( signed_unary_expValue.value.c_str() ) ;
        tempNUM = -tempNUM ;
        sstream << tempNUM ;
        signed_unary_expValue.value = sstream.str() ;
      } // else if
      
      
      romce_and_romloeValue = signed_unary_expValue ;
      Romce_and_romloe( romce_and_romloeCorrect, romce_and_romloeValue ) ;
      
      
      if ( romce_and_romloeCorrect == false ) {
        correct = false ;
        return ;
      } // if
      
      correct = true ;
      value = romce_and_romloeValue ;
      return ;
        
    } // else
    
    correct = false ;
    return ;
    
  } // Basic_expression()
  
  void Rest_of_Identifier_started_basic_exp( bool &correct, Formula &value, string id ) {
  // rest_of_Identifier_started_basic_exp
  // : [ '[' expression ']' ]
  //   ( assignment_operator basic_expression 
  //     | 
  //     [ PP | MM ] romce_and_romloe 
  //    )
  //   | '(' [ actual_parameter_list ] ')' romce_and_romloe
    
    int tokenType ;
    string tokenValue ;
    bool actual_parameter_listCorrect = false ;
    Formula actual_parameter_listValue ;
    bool romce_and_romloeCorrect = false ;
    Formula romce_and_romloeValue ;
    bool expressionCorrect = false ;
    Formula expresssionValue ;
    bool assignment_operatorCorrect = false ;
    int assignment_operatorValue ;
    bool basic_expressionCorrect = false ;
    Formula basic_expressionValue ;
    Formula temp ;
    double tempNUM ;
    stringstream sstream;
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == LP )  {                     // '('
      
      GetToken( tokenType, tokenValue ) ;
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == Identifier  || tokenType == PP || tokenType == MM 
           || tokenType == LP || tokenType == Constant || tokenType == PLUS 
           || tokenType == SUB || tokenType == EM ) {
        
        Actual_parameter_list( actual_parameter_listCorrect, actual_parameter_listValue ) ;
        
        if ( actual_parameter_listCorrect == false ) {
          correct = false ;
          return ;
        } // if

      } // if
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != RP ) {                  // ')'
        correct = false ;
        return;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Romce_and_romloe( romce_and_romloeCorrect, romce_and_romloeValue ) ;
      
      if ( romce_and_romloeCorrect == false ) {
        correct = false ;
        return;
      } // if
      
      correct = true ;
      return;
          
    } // if
    else {
      
      PeekToken( tokenType, tokenValue ) ;

      if ( tokenType == LSB ) {                              // '[' 
          
        GetToken( tokenType, tokenValue ) ;
          
        Expression( expressionCorrect, expresssionValue ) ;
          
        if ( expressionCorrect == false ) {
          correct = false ;
          return ;    
        } // if
        
        PeekToken( tokenType, tokenValue );
        
        if ( tokenType != RSB ) {                          // ']'
          correct = false ;
          return ;    
        } // if
        
        GetToken( tokenType, tokenValue ) ; 
          
      }   // if
      
      SearchArray( id, expresssionValue, temp ) ;             // 找出陣列的值 
      
      Assignment_operator( assignment_operatorCorrect, assignment_operatorValue ) ; 
      
      if ( assignment_operatorCorrect == true ) {

        Basic_expression( basic_expressionCorrect, basic_expressionValue ) ;
        
        if ( basic_expressionCorrect == false ) {
          correct = false ;
          return ;
        } // if
        
        if ( assignment_operatorValue != assignment ) 
          Compute( temp, basic_expressionValue, assignment_operatorValue ) ;
        else temp = basic_expressionValue ;
        
        Assign_Var( id, expresssionValue, temp ) ; 
        
        correct = true ;
        value = temp ;
        return ;
        
      } // if
      else {
        
        
        PeekToken( tokenType, tokenValue ) ;
          
        if ( tokenType == PP ) {              // '++', '--'
          GetToken( tokenType, tokenValue ) ;
        } // if
        else if ( tokenType == MM ) {        
          GetToken( tokenType, tokenValue ) ;
        } // if     

        romce_and_romloeValue = temp ; 
        
        if ( tokenType == PP ) {              // '++', '--'
          tempNUM = atof( temp.value.c_str() ) ;
          tempNUM++ ;
          sstream << tempNUM ;
          temp.value = sstream.str() ;
          Assign_Var( id, expresssionValue, temp ) ;
        } // if
        else if ( tokenType == MM ) {        
          tempNUM = atof( temp.value.c_str() ) ;
          tempNUM-- ;
          sstream << tempNUM ;
          temp.value = sstream.str() ;
          Assign_Var( id, expresssionValue, temp ) ;
        } // if 
        
        Romce_and_romloe( romce_and_romloeCorrect, romce_and_romloeValue ) ;              
         
        if ( romce_and_romloeCorrect == false ) {
          correct = false ;
          return ;    
        } // if   

        
          
        correct = true ;
        value = romce_and_romloeValue ;
       
        return ;
        
      } // else
        
    } // else
    
    
  } // Rest_of_Identifier_started_basic_exp()
  
  void Rest_of_PPMM_Identifier_started_basic_exp( bool &correct, Formula &value, string id, int type ) {
  // rest_of_PPMM_Identifier_started_basic_exp
  // : [ '[' expression ']' ] romce_and_romloe
    
    int tokenType ;
    string tokenValue ;
    bool expressionCorrect = false ;
    Formula expressionValue ; 
    bool romce_and_romloeCorrect = false ;
    Formula romce_and_romloeValue ; 
    Formula temp ;
    double tempNUM ;
    stringstream sstream;
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == LSB )  {                                  // '['
        
      GetToken( tokenType, tokenValue ) ;    
        
      Expression( expressionCorrect, expressionValue ) ;
      
      if ( expressionCorrect == false ) {
        correct = false ;
        return ;
      } // if
      
      PeekToken( tokenType, tokenValue ) ; 
      
      if ( tokenType != RSB )  {                                 // ']'
        correct = false ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
        
    } // if
    
    SearchArray( id, expressionValue, temp ) ;             // 找出陣列的值 
    
    if ( type == PP ) {              // '++', '--'
      tempNUM = atof( temp.value.c_str() ) ;
      tempNUM++ ;
      sstream << tempNUM ;
      temp.value = sstream.str() ;
      Assign_Var( id, expressionValue, temp ) ;
    } // if
    else if ( type == MM ) {        
      tempNUM = atof( temp.value.c_str() ) ;
      tempNUM-- ;
      sstream << tempNUM ;
      temp.value = sstream.str() ;
      Assign_Var( id, expressionValue, temp ) ;
    } // if 
    
    romce_and_romloeValue = temp ;
    
    Romce_and_romloe( romce_and_romloeCorrect, romce_and_romloeValue ) ;
    
    if ( romce_and_romloeCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    
    correct = true ;
    value = romce_and_romloeValue ;
    return ;
     
  } // Rest_of_PPMM_Identifier_started_basic_exp()
  
  void Sign( bool &correct, string &value ) { 
  // sign
  // : '+' | '-' | '!'
  
    int tokenType ;
    string tokenValue ;
  
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == PLUS || tokenType == SUB || tokenType == EM ) {        // '+','-','!'
      
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      value = tokenValue;
      return ;
      
    } // if
    else {
      correct = false ;
      return ;
    } // else 

  } // Sign()
  
  void Actual_parameter_list( bool &correct, Formula &value ) {
  // actual_parameter_list 
  // : basic_expression { ',' basic_expression }
    
    int tokenType ;
    string tokenValue ;
    bool basic_expression1Correct = false ;
    Formula basic_expression1Value ;
    bool basic_expression2Correct = false ;
    Formula basic_expression2Value ;
    
    Basic_expression( basic_expression1Correct, basic_expression1Value ) ;
    
    if ( basic_expression1Correct == false ) {
      correct = false ;
      return ;    
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != COMMA ) {
        correct = true ;
        value = basic_expression1Value ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Basic_expression( basic_expression2Correct, basic_expression2Value ) ;
    
      if ( basic_expression2Correct == false ) {
        correct = false ;
        return ;    
      } // if
        
    } while ( true ) ;
    
  
  } // Actual_parameter_list()
  
  void Assignment_operator( bool &correct, int &value ) {
  // assignment_operator
  // : '=' | TE | DE | RE | PE | ME
    
    int tokenType ;
    string tokenValue ;
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == assignment || tokenType == TE || tokenType == DE            // '=','*=','/='
         || tokenType == RE || tokenType == PE || tokenType == ME )  {              // '%=','+=','-='
         
      GetToken( tokenType, tokenValue ) ;   
      
      correct = true ;
      value = tokenType ;
      return ;
      
    } // if 
  
    
    correct = false ;
    return ;
  
  } // Assignment_operator()
  
  void Romce_and_romloe( bool &correct, Formula &value ) { // 即 romce_and_romloe
  // rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp
  // : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
    
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_logical_OR_expCorrect = false ;
    Formula rest_of_maybe_logical_OR_expValue = value ;
    bool basic_expression1Correct = false ;
    Formula basic_expression1Value ;
    bool basic_expression2Correct = false ;
    Formula basic_expression2Value ;
    vector<Var>periodVar  ;
    vector<Var>temp1Var ;
    vector<Var>temp2Var ;
    periodVar = mVarDef ;
    
    Rest_of_maybe_logical_OR_exp( rest_of_maybe_logical_OR_expCorrect,
                                  rest_of_maybe_logical_OR_expValue ) ;
                                  
    if ( rest_of_maybe_logical_OR_expCorrect == false ) {                          
      correct = false ;
      return ;
    } // if                               
    
    PeekToken( tokenType, tokenValue ) ;
    
    
    if ( tokenType != QM ) {                          // '?'
      correct = true ;
      value = rest_of_maybe_logical_OR_expValue ;
      return ;
    } // if
    
    GetToken( tokenType, tokenValue ) ;
    
    Basic_expression( basic_expression1Correct, basic_expression1Value ) ;
    
    if ( basic_expression1Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    temp1Var = mVarDef ;
    mVarDef = periodVar ;
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType != COLON ) {       //  ':'
      correct = false ;
      return ;
    } // if
    
    GetToken( tokenType, tokenValue ) ;
  
    Basic_expression( basic_expression2Correct, basic_expression2Value ) ;
    
    if ( basic_expression2Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    temp2Var = mVarDef ;
    correct = true ;
    
    if ( rest_of_maybe_logical_OR_expValue.value == "true" ) {
      value = basic_expression1Value ;
      mVarDef = temp1Var ;
    } // if
    else if ( rest_of_maybe_logical_OR_expValue.value == "false" ) {
      value = basic_expression2Value ;
      mVarDef = temp2Var ;
    } // else if
    
    return ;
  
  
  } // Romce_and_romloe()
  
  void Rest_of_maybe_logical_OR_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_logical_OR_exp 
  // : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
  
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_logical_AND_expCorrect = false ;
    Formula rest_of_maybe_logical_AND_expValue = value ;
    bool maybe_logical_AND_expCorrect = false ;
    Formula maybe_logical_AND_expValue ;
    
    Rest_of_maybe_logical_AND_exp( rest_of_maybe_logical_AND_expCorrect,
                                   rest_of_maybe_logical_AND_expValue )  ;
    
    if ( rest_of_maybe_logical_AND_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != OR ) {            // '||'
        correct = true ;
        value = rest_of_maybe_logical_AND_expValue ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_logical_AND_exp( maybe_logical_AND_expCorrect, maybe_logical_AND_expValue ) ;
      
      if ( maybe_logical_AND_expCorrect == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( rest_of_maybe_logical_AND_expValue, maybe_logical_AND_expValue, tokenType ) ;
      
    } while ( true ) ;
  
  } // Rest_of_maybe_logical_OR_exp()
  
  void Maybe_logical_AND_exp( bool &correct, Formula &value ) {
  // maybe_logical_AND_exp 
  // : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    
    int tokenType ;
    string tokenValue ;
    bool maybe_bit_OR_exp1Correct = false ;
    Formula maybe_bit_OR_exp1Value ;
    bool maybe_bit_OR_exp2Correct = false ;
    Formula maybe_bit_OR_exp2Value ;
    
    Maybe_bit_OR_exp( maybe_bit_OR_exp1Correct, maybe_bit_OR_exp1Value )  ;
    
    if ( maybe_bit_OR_exp1Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      
      if ( tokenType != AND ) {            // '&&'
        correct = true ;
        value = maybe_bit_OR_exp1Value ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_bit_OR_exp( maybe_bit_OR_exp2Correct, maybe_bit_OR_exp2Value ) ;
      
      if ( maybe_bit_OR_exp2Correct == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( maybe_bit_OR_exp1Value, maybe_bit_OR_exp2Value, tokenType ) ;
        
    } while ( true )  ;
    
  } // Maybe_logical_AND_exp()
  
  void Rest_of_maybe_logical_AND_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_logical_AND_exp 
  // : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_bit_OR_expCorrect = false ;
    Formula rest_of_maybe_bit_OR_expValue = value  ;
    bool maybe_bit_OR_expCorrect = false ;
    Formula maybe_bit_OR_expValue ;
    
    Rest_of_maybe_bit_OR_exp( rest_of_maybe_bit_OR_expCorrect, rest_of_maybe_bit_OR_expValue ) ;
    
    if ( rest_of_maybe_bit_OR_expCorrect == false ) {
      correct = false ;
      value = rest_of_maybe_bit_OR_expValue ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
       
      if ( tokenType != AND ) {            // '&&'
        correct = true ;
        value = rest_of_maybe_bit_OR_expValue ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_bit_OR_exp( maybe_bit_OR_expCorrect, maybe_bit_OR_expValue ) ;
      
      if ( maybe_bit_OR_expCorrect == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( rest_of_maybe_bit_OR_expValue, maybe_bit_OR_expValue, tokenType ) ;
        
    } while ( true ) ;
    
  } // Rest_of_maybe_logical_AND_exp()
  
  void Maybe_bit_OR_exp( bool &correct, Formula &value ) {
  // maybe_bit_OR_exp 
  // : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }

    int tokenType ;
    string tokenValue ;
    bool maybe_bit_ex_OR_exp1Correct = false ;
    Formula maybe_bit_ex_OR_exp1Value ;
    bool maybe_bit_ex_OR_exp2Correct = false ;
    Formula maybe_bit_ex_OR_exp2Value  ;
    
    Maybe_bit_ex_OR_exp( maybe_bit_ex_OR_exp1Correct, maybe_bit_ex_OR_exp1Value ) ;
    
    if ( maybe_bit_ex_OR_exp1Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != BOr ) {            // '|'
        correct = true ;
        value = maybe_bit_ex_OR_exp1Value ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_bit_ex_OR_exp( maybe_bit_ex_OR_exp2Correct, maybe_bit_ex_OR_exp2Value ) ;
      
      if ( maybe_bit_ex_OR_exp2Correct == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( maybe_bit_ex_OR_exp1Value, maybe_bit_ex_OR_exp2Value, tokenType ) ;
        
    } while ( true )  ;
    
  } // Maybe_bit_OR_exp()
  
  void Rest_of_maybe_bit_OR_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_bit_OR_exp 
  // : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
    
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_bit_ex_OR_expCorrect = false ;
    Formula rest_of_maybe_bit_ex_OR_expValue = value ;
    bool maybe_bit_ex_OR_expCorrect = false ;
    Formula maybe_bit_ex_OR_expValue ;
    
    Rest_of_maybe_bit_ex_OR_exp( rest_of_maybe_bit_ex_OR_expCorrect, rest_of_maybe_bit_ex_OR_expValue ) ;
    
    if ( rest_of_maybe_bit_ex_OR_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != BOr ) {            // '|'
        correct = true ;
        value = rest_of_maybe_bit_ex_OR_expValue ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_bit_ex_OR_exp( maybe_bit_ex_OR_expCorrect, maybe_bit_ex_OR_expValue ) ;
      
      if ( maybe_bit_ex_OR_expCorrect == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( rest_of_maybe_bit_ex_OR_expValue, maybe_bit_ex_OR_expValue, tokenType ) ;
        
    } while ( true )   ;
  
  
  } // Rest_of_maybe_bit_OR_exp()
  
  void Maybe_bit_ex_OR_exp( bool &correct, Formula &value ) {
  //  maybe_bit_ex_OR_exp 
  //  : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  
    int tokenType ;
    string tokenValue ;
    bool maybe_bit_AND_exp1Correct = false ;
    Formula maybe_bit_AND_exp1Value ;
    bool maybe_bit_AND_exp2Correct = false ;
    Formula maybe_bit_AND_exp2Value ;
    
    Maybe_bit_AND_exp( maybe_bit_AND_exp1Correct, maybe_bit_AND_exp1Value ) ;
    
    if ( maybe_bit_AND_exp1Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != XOR ) {            // '^'
        correct = true ;
        value = maybe_bit_AND_exp1Value ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_bit_AND_exp( maybe_bit_AND_exp2Correct, maybe_bit_AND_exp2Value ) ;
      
      if ( maybe_bit_AND_exp2Correct == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( maybe_bit_AND_exp1Value, maybe_bit_AND_exp2Value, tokenType ) ;
        
    } while ( true )  ;
  
  
  } // Maybe_bit_ex_OR_exp()
  
  void Rest_of_maybe_bit_ex_OR_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_bit_ex_OR_exp 
  // : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_bit_AND_expCorrect = false ;
    Formula rest_of_maybe_bit_AND_expValue = value ;
    bool maybe_bit_AND_expCorrect = false ;
    Formula maybe_bit_AND_expValue ;
    
    Rest_of_maybe_bit_AND_exp( rest_of_maybe_bit_AND_expCorrect, rest_of_maybe_bit_AND_expValue ) ;
    
    if ( rest_of_maybe_bit_AND_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != XOR ) {                      // '^'
        correct = true ;
        value = rest_of_maybe_bit_AND_expValue ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_bit_AND_exp( maybe_bit_AND_expCorrect, maybe_bit_AND_expValue ) ;
      
      if ( maybe_bit_AND_expCorrect == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( rest_of_maybe_bit_AND_expValue, maybe_bit_AND_expValue, tokenType ) ;
        
    } while ( true )  ;
  
  } // Rest_of_maybe_bit_ex_OR_exp()
  
  void Maybe_bit_AND_exp( bool &correct, Formula &value ) {
  // maybe_bit_AND_exp 
  // : maybe_equality_exp { '&' maybe_equality_exp }
  
    int tokenType ;
    string tokenValue ;
    bool maybe_equality_exp1Correct = false ;
    Formula maybe_equality_exp1Value ;
    bool maybe_equality_exp2Correct = false ;
    Formula maybe_equality_exp2Value ;
    
    Maybe_equality_exp( maybe_equality_exp1Correct, maybe_equality_exp1Value ) ;
    
    if ( maybe_equality_exp1Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != BAnd ) {              // '&'
        correct = true ;
        value = maybe_equality_exp1Value ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_equality_exp( maybe_equality_exp2Correct, maybe_equality_exp2Value ) ;
      
      if ( maybe_equality_exp2Correct == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( maybe_equality_exp1Value, maybe_equality_exp2Value, tokenType ) ;
        
    } while ( true ) ;
    
  } // Maybe_bit_AND_exp()
  
  void Rest_of_maybe_bit_AND_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_bit_AND_exp 
  // : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
  
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_equality_expCorrect = false ;
    Formula rest_of_maybe_equality_expValue = value ;
    bool maybe_equality_expCorrect = false ;
    Formula maybe_equality_expValue ;
    
    Rest_of_maybe_equality_exp( rest_of_maybe_equality_expCorrect, rest_of_maybe_equality_expValue ) ;
    
    if ( rest_of_maybe_equality_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue  ) ;
      
      if ( tokenType != BAnd ) {              // '&'
        correct = true ;
        value =  rest_of_maybe_equality_expValue ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      Maybe_equality_exp( maybe_equality_expCorrect, maybe_equality_expValue ) ;
      
      if ( maybe_equality_expCorrect == false ) {
        correct = false;
        return ;
      } // if
      
      Compute( rest_of_maybe_equality_expValue, maybe_equality_expValue, tokenType ) ;
        
    } while ( true )  ;
    
  } // Rest_of_maybe_bit_AND_exp()
  
  void Maybe_equality_exp( bool &correct, Formula &value ) {
  // maybe_equality_exp 
  // : maybe_relational_exp 
  // { ( EQ | NEQ ) maybe_relational_exp } 
  
    int tokenType ;
    string tokenValue ;
    bool maybe_relational_exp1Correct = false ;
    Formula maybe_relational_exp1Value ;
    bool maybe_relational_exp2Correct = false ;
    Formula maybe_relational_exp2Value ;
    
    Maybe_relational_exp( maybe_relational_exp1Correct, maybe_relational_exp1Value ) ;
    
    if ( maybe_relational_exp1Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == EQ || tokenType == NEQ ) {
          
        GetToken( tokenType, tokenValue ) ;
          
        Maybe_relational_exp( maybe_relational_exp2Correct, maybe_relational_exp2Value ) ;
    
        if ( maybe_relational_exp2Correct == false ) {
          correct = false ;
          return ;
        } // if
        
        Compute( maybe_relational_exp1Value, maybe_relational_exp2Value, tokenType ) ;
          
          
      } // if
      else {
        correct = true ;
        value = maybe_relational_exp1Value ;
        return ;
      } // else
      
    } while ( true ) ;
  
  
  } // Maybe_equality_exp()
  
  void Rest_of_maybe_equality_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_equality_exp 
  // : rest_of_maybe_relational_exp 
  // { ( EQ | NEQ ) maybe_relational_exp }
    
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_relational_expCorrect = false ;
    Formula rest_of_maybe_relational_expValue = value  ;
    bool maybe_relational_expCorrect = false ;
    Formula maybe_relational_expValue ;
    
    Rest_of_maybe_relational_exp( rest_of_maybe_relational_expCorrect,
                                  rest_of_maybe_relational_expValue ) ;
    
    
    
    if ( rest_of_maybe_relational_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == EQ || tokenType == NEQ ) {
          
        GetToken( tokenType, tokenValue ) ;
          
        Maybe_relational_exp( maybe_relational_expCorrect, maybe_relational_expValue ) ;
    
        if ( maybe_relational_expCorrect == false ) {
          correct = false ;
          return ;
        } // if
         
        Compute( rest_of_maybe_relational_expValue, maybe_relational_expValue, tokenType ) ;  
          
      } // if
      else {
        correct = true ;
        value = rest_of_maybe_relational_expValue ;
        return ; 
      } // else
      
      
    } while ( true ) ;
    
    
  } // Rest_of_maybe_equality_exp()
  
  void Maybe_relational_exp( bool &correct, Formula &value ) { 
  // maybe_relational_exp 
  // : maybe_shift_exp 
  // { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  
  
    int tokenType ;
    string tokenValue ;
    bool maybe_shift_exp1Correct = false ;
    Formula maybe_shift_exp1Value ;
    bool maybe_shift_exp2Correct = false ;
    Formula maybe_shift_exp2Value ;
    
    Maybe_shift_exp( maybe_shift_exp1Correct, maybe_shift_exp1Value ) ;
    
    if ( maybe_shift_exp1Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == LT || tokenType == GT || tokenType == LE || tokenType == GE ) {    
           // '<','>','<=','>='
          
        GetToken( tokenType, tokenValue ) ;
          
        Maybe_shift_exp( maybe_shift_exp2Correct, maybe_shift_exp2Value ) ;
    
        if ( maybe_shift_exp2Correct == false ) {
          correct = false ;
          return ;
        } // if
         
        Compute( maybe_shift_exp1Value, maybe_shift_exp2Value, tokenType ) ;  
          
      } // if
      else {
        correct = true ;
        value = maybe_shift_exp1Value ;
        return ;        
      } // else
      
    } while ( true ) ;
  
  
  } // Maybe_relational_exp()
  
  
  void Rest_of_maybe_relational_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_relational_exp 
  // : rest_of_maybe_shift_exp 
  // { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  
  
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_shift_expCorrect = false ;
    Formula rest_of_maybe_shift_expValue = value ;
    bool maybe_shift_expCorrect = false ;
    Formula maybe_shift_expValue ;
    
    Rest_of_maybe_shift_exp( rest_of_maybe_shift_expCorrect, rest_of_maybe_shift_expValue ) ;
    
    if ( rest_of_maybe_shift_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == LT || tokenType == GT || tokenType == LE || tokenType == GE ) { 
         // '<','>','<=','>='
          
        GetToken( tokenType, tokenValue ) ;
          
        Maybe_shift_exp( maybe_shift_expCorrect, maybe_shift_expValue ) ;
    
        if ( maybe_shift_expCorrect == false ) {
          correct = false ;
          return ;
        } // if
        
        Compute( rest_of_maybe_shift_expValue, maybe_shift_expValue, tokenType ) ;
          
      } // if
      else {
        correct = true ;
        value = rest_of_maybe_shift_expValue ;
        return ;
      } // else
      
    } while ( true ) ;
    
  } // Rest_of_maybe_relational_exp()
  
  void Maybe_shift_exp( bool &correct, Formula &value ) { 
  // maybe_shift_exp 
  // : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
    
    int tokenType ;
    string tokenValue ;
    bool maybe_additive_exp1Correct = false ;
    Formula maybe_additive_exp1Value ;
    bool maybe_additive_exp2Correct = false ;
    Formula maybe_additive_exp2Value ;
    bool isCout = false ;
    bool isCin = false ;
    
    Maybe_additive_exp( maybe_additive_exp1Correct, maybe_additive_exp1Value ) ;
    
    if ( maybe_additive_exp1Correct == false ) {
      correct = false ;
      return ;
    } // if
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == LS || tokenType == RS ) {                           // '<<','>>'
        
        if ( mTempFun[mTempFun.size()-1] == "cin" ) 
          isCin = true ;
        else if ( mTempFun[mTempFun.size()-1] == "cout" ) 
          isCout = true ;
        
        
        if ( isCin == true && tokenType == LS ) {
          correct = false;
          return ;
        } // if
        else if ( isCout == true && tokenType == RS ) {
          correct = false;
          return ;
        } // else if
        
        GetToken( tokenType, tokenValue ) ;
          
        Maybe_additive_exp( maybe_additive_exp2Correct, maybe_additive_exp2Value ) ;
    
        if ( maybe_additive_exp2Correct == false ) {
          correct = false ;
          return ;
        } // if
          
        Compute( maybe_additive_exp1Value, maybe_additive_exp2Value, tokenType ) ;
        
        if ( isCout == true ) 
          mOutput.push_back( maybe_additive_exp1Value ) ;

      } // if
      else {    
        correct = true ;
        value = maybe_additive_exp1Value ;
        return ;
      } // else
      
    } while ( true ) ;
  
  } // Maybe_shift_exp()
  
  void Rest_of_maybe_shift_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_shift_exp 
  // : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }    
    
    int tokenType ;
    string tokenValue ; 
    bool rest_of_maybe_additive_expCorrect = false ;
    Formula rest_of_maybe_additive_expValue = value ;
    bool maybe_additive_expCorrect = false ;
    Formula maybe_additive_expValue ;
    bool isCout = false ;
    bool isCin = false ;
    
    Rest_of_maybe_additive_exp( rest_of_maybe_additive_expCorrect, rest_of_maybe_additive_expValue ) ;
    
    if ( rest_of_maybe_additive_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    
    do {
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == LS || tokenType == RS ) {                           // '<<','>>'
        
        
        if ( mTempFun[mTempFun.size()-1] == "cin" ) 
          isCin = true ;
        else if ( mTempFun[mTempFun.size()-1] == "cout" ) 
          isCout = true ;
        
        
        if ( isCin == true && tokenType == LS ) {
          correct = false;
          return ;
        } // if
        else if ( isCout == true && tokenType == RS ) {
          correct = false;
          return ;
        } // else if
        
        GetToken( tokenType, tokenValue ) ;
          
        Maybe_additive_exp( maybe_additive_expCorrect, maybe_additive_expValue ) ;
        
        if ( maybe_additive_expCorrect == false ) {
          correct = false ;
          return ;
        } // if
        
        
         
        if ( isCout == true ) {
          mOutput.push_back( maybe_additive_expValue ) ;
        }  // if
        else Compute( rest_of_maybe_additive_expValue, maybe_additive_expValue, tokenType ) ;
        
        
      } // if
      else {
        correct = true ;
        value = rest_of_maybe_additive_expValue ;
        return ;
      } // else
      
    } while ( true ) ;
  
  } // Rest_of_maybe_shift_exp()
  
  void Maybe_additive_exp( bool &correct, Formula &value ) {
  // maybe_additive_exp 
  // : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  
    int tokenType ;
    string tokenValue ;
    bool maybe_mult_exp1Correct = false ;
    Formula maybe_mult_exp1Value ;
    bool maybe_mult_exp2Correct = false ;
    Formula maybe_mult_exp2Value ;
    
    
    Maybe_mult_exp( maybe_mult_exp1Correct, maybe_mult_exp1Value ) ;
    
    if ( maybe_mult_exp1Correct == false ) {
      correct = false;
      return ;
    } // if
    
    
    do {
    
      PeekToken( tokenType, tokenValue ) ;
     
      if ( tokenType == PLUS || tokenType == SUB ) {                           // '+','-'
          
        GetToken( tokenType, tokenValue ) ;
          
        Maybe_mult_exp( maybe_mult_exp2Correct, maybe_mult_exp2Value ) ;
    
        if ( maybe_mult_exp2Correct == false ) {
          correct = false ;
          return ;
        } // if
        
        
        Compute( maybe_mult_exp1Value, maybe_mult_exp2Value, tokenType ) ;
        
        
      } // if
      else {
        correct = true ;
        value = maybe_mult_exp1Value ;
        return ;
      } // else    
        
    } while ( true ) ;
    
  } // Maybe_additive_exp()
  
  void Rest_of_maybe_additive_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_additive_exp 
  // : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  
    int tokenType ;
    string tokenValue ;
    bool rest_of_maybe_mult_expCorrect = false ;
    Formula rest_of_maybe_mult_expValue = value ;
    bool maybe_mult_expCorrect = false ;
    Formula maybe_mult_expValue ;
    
    
    Rest_of_maybe_mult_exp( rest_of_maybe_mult_expCorrect, rest_of_maybe_mult_expValue ) ;
    
    if ( rest_of_maybe_mult_expCorrect == false ) {
      correct = false;
      return ;
    } // if
    
    
    do {
    
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == PLUS || tokenType == SUB ) {                           // '+','-'
          
        GetToken( tokenType, tokenValue ) ;
        
        Maybe_mult_exp( maybe_mult_expCorrect, maybe_mult_expValue ) ;
        
        if ( maybe_mult_expCorrect == false ) {
          correct = false ;
          return ;
        } // if
        
        
        Compute( rest_of_maybe_mult_expValue, maybe_mult_expValue, tokenType ) ;
        
      } // if
      else {
        correct = true ;
        value = rest_of_maybe_mult_expValue ;
        return ;
      } // else    
        
    } while ( true ) ;
    
  } // Rest_of_maybe_additive_exp()
  
  void Maybe_mult_exp( bool &correct, Formula &value ) {
  // maybe_mult_exp 
  // : unary_exp rest_of_maybe_mult_exp
  
    bool unary_expCorrect = false ;
    Formula unary_expValue ;
    bool rest_of_maybe_mult_expCorrect = false ;
    Formula rest_of_maybe_mult_expValue ;
    
    Unary_exp( unary_expCorrect, unary_expValue ) ;
    
    if ( unary_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
    rest_of_maybe_mult_expValue = unary_expValue ;
    
    Rest_of_maybe_mult_exp( rest_of_maybe_mult_expCorrect, rest_of_maybe_mult_expValue ) ;
    
    if ( rest_of_maybe_mult_expCorrect == false ) {
      correct = false ;
      return ;
    } // if
    
      
    correct = true ;
    value = rest_of_maybe_mult_expValue ;
    return ;
    
  } // Maybe_mult_exp()
  
  void Rest_of_maybe_mult_exp( bool &correct, Formula &value ) {
  // rest_of_maybe_mult_exp 
  // : { ( '*' | '/' | '%' ) unary_exp } /* could be empty ! */
  
    int tokenType ;
    string tokenValue ;
    bool unary_expCorrect = false ;
    Formula unary_expValue ;
    Formula temp = value ;
    
    
    
    do {
        
      PeekToken( tokenType, tokenValue ) ;
    
      if ( tokenType == MULT || tokenType == DIV || tokenType == remainder )  {                              
          // '*', '/', '%'
    
        GetToken( tokenType, tokenValue ) ;

        Unary_exp( unary_expCorrect, unary_expValue ) ;
        
        if ( unary_expCorrect == false ) {
          correct = false ;
          return ;
        } // if
        
        Compute( temp, unary_expValue, tokenType ) ;
      
      } // if
      else {
        correct = true ;
        value = temp ;
        return ;
      } // else
  
    } while ( true ) ;
    
    correct = true;
    return ;
    
    
  }  // Rest_of_maybe_mult_exp()
  
  void Unary_exp( bool &correct, Formula &value ) {
  // unary_exp
  // : sign { sign } signed_unary_exp
  // | unsigned_unary_exp
  // | ( PP | MM ) Identifier [ '[' expression ']' ]
  
    int tokenType ;
    string tokenValue ;
    bool expressionCorrect ;
    Formula expressionValue ;
    bool signCorrect = false ;
    string signValue ;
    bool unsigned_unary_expCorrect = false ;
    Formula unsigned_unary_expValue ;
    bool signed_unary_expCorrect = false ;
    Formula signed_unary_expValue ;
    bool repeat = true ;
    int num = 0 ;        // 檢驗正負數 
    int number = 0 ;      // 檢驗bool 反轉幾次 
    double tempNUM ;
    string idName ;
    Formula temp ;
    stringstream sstream;
    int isPlus = 0 ;
    
    Sign( signCorrect, signValue ) ;
      
    if ( signCorrect == true ) {

      do {

        if ( signValue == "+" )  num++ ;
        else if ( signValue == "-" ) num-- ;
        else if ( signValue == "!" ) number++ ;
        
        Sign( signCorrect, signValue ) ;    
          
        if ( signCorrect == false ) repeat = false ;
        
      } while ( repeat == true )    ;
       
      Signed_unary_exp( signed_unary_expCorrect, signed_unary_expValue ) ;
        
      if ( signed_unary_expCorrect == false ) {
        correct = false ;
        return ;
      } // if
        
      correct = true ;
      
      
      if ( number % 2 != 0 ) {

        if ( signed_unary_expValue.value == "true" ) 
          signed_unary_expValue.value = "false" ; 
        else if ( signed_unary_expValue.value == "false" ) {
          signed_unary_expValue.value = "true" ;
        } // else if

        value = signed_unary_expValue ;

      } // if
      else if ( num >= 0 )  value = signed_unary_expValue ;
      else if ( num < 0 ) {
        tempNUM = atof( signed_unary_expValue.value.c_str() ) ;
        tempNUM = -tempNUM ;
        sstream << tempNUM ;
        signed_unary_expValue.value = sstream.str() ;
        value = signed_unary_expValue ;
      } // else if
      
      
      return ;
      
    } // if
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == Identifier || tokenType == Constant 
         || tokenType == LP ) {

      Unsigned_unary_exp( unsigned_unary_expCorrect, unsigned_unary_expValue ) ;
          
      if ( unsigned_unary_expCorrect == false ) {
        correct = false ;
        return ;   
      } // if
      else {
        correct = true ;
        value = unsigned_unary_expValue ;
        return ;
      } // else
  
    } // if
    
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == PP || tokenType == MM ) {                     // '++', '--'
      
      isPlus = tokenType ;
      
      GetToken( tokenType, tokenValue ) ;
        
      PeekToken( tokenType, tokenValue ) ;
        
      if ( tokenType != Identifier ) {
        correct = false ;
        return ;
      } // if
        
      if ( CheckID( tokenValue ) == false ) {
        mNotDef = true ;
        correct = false ;
        return ;
      }  // if
      
      idName = tokenValue ;  
      GetToken( tokenType, tokenValue ) ;
      
      PeekToken( tokenType, tokenValue ) ;
      
      
      if ( tokenType != LSB ) {                                                       // '['
        correct = true ;
        SearchArray( idName, expressionValue, temp ) ;
        tempNUM = atof( temp.value.c_str() ) ;
        if ( isPlus == PP ) {
          tempNUM++ ;
          sstream << tempNUM ;
          temp.value = sstream.str() ;
          Assign_Var( idName, expressionValue, temp ) ;
        } // if
        else {
          tempNUM-- ;
          sstream << tempNUM ;
          temp.value = sstream.str() ;
          Assign_Var( idName, expressionValue, temp ) ;
        }  // else
        
        
        value = temp ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
        
      Expression( expressionCorrect, expressionValue ) ;
      
      if ( expressionCorrect == false ) {
        correct = false ;
        return ;
      }     // if
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != RSB ) {                                                       // ']'
        correct = false ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      SearchArray( idName, expressionValue, temp ) ;             // 找出陣列的值 
      
      tempNUM = atof( temp.value.c_str() ) ;
      
      if ( isPlus == PP ) {
        tempNUM++ ;
        sstream << tempNUM ;
        temp.value = sstream.str() ;
        Assign_Var( idName, expressionValue, temp ) ;
      } // if
      else if ( isPlus == MM ) {
        tempNUM-- ;
        sstream << tempNUM ;
        temp.value = sstream.str() ;
        Assign_Var( idName, expressionValue, temp ) ;
      }  // else if
      
      
      correct = true ;
      value = temp ;
      return ;
      
    } // if
    
    correct = false ;
    return ;
  
  } // Unary_exp()
  
  void Signed_unary_exp( bool &correct, Formula &value ) {
  // signed_unary_exp
  // : Identifier [ '(' [ actual_parameter_list ] ')' 
  // | '[' expression ']' ] | Constant | '(' expression ')'
    
    int tokenType ;
    string tokenValue ;
    bool actual_parameter_listCorrect = false ;
    Formula actual_parameter_listValue ;
    bool expressionCorrect = false ;
    Formula expressionValue ;
    Formula temp ;
    string idName ;
    bool isFloat = false ;
  
    PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenType == Identifier ) {
        
      if ( CheckID( tokenValue ) == false ) {
        mNotDef = true ;
        correct = false ;
        return ;
      }  // if
      
      
      GetToken( tokenType, tokenValue ) ;
      idName = tokenValue ;
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == LP ) {             // '('    
          
        GetToken( tokenType, tokenValue ) ;
          
        Actual_parameter_list( actual_parameter_listCorrect, actual_parameter_listValue ) ;
        
        PeekToken( tokenType, tokenValue ) ;
        
        if ( tokenType != RP ) {                      //    ')'
          correct = false ;
          return ;
        } // if
        
        GetToken( tokenType, tokenValue ) ;
        
        correct = true ;
        return ;
          
          
      } // if
      else if ( tokenType == LSB )  {             //  '['
      
        GetToken( tokenType, tokenValue ) ;
        
        Expression( expressionCorrect, expressionValue ) ;
        
        if ( expressionCorrect == false ) {
          correct = false ;
          return ;
        } // if
        
        PeekToken( tokenType, tokenValue ) ;
        
        if ( tokenType != RSB ) {              // ']'
          correct = false ;
          return ;
        } // if
        
        SearchArray( idName, expressionValue, temp ) ;             // 找出陣列的值 
        
        GetToken( tokenType, tokenValue ) ;
        
        correct = true ;
        value = temp ;
        return ;
          
      } // else if
      else {
        SearchArray( idName, expressionValue, temp ) ;             // ID 的值 
        correct = true ;
        value = temp ;
        return ;
      } // else
        
    } // if
    else if ( tokenType == LP ) {                          // '('
        
      GetToken( tokenType, tokenValue ) ;
      
      Expression( expressionCorrect, expressionValue ) ;
      
      if ( expressionCorrect == false ) {
        correct = false ;
        return ;
      } // if
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != RP ) {                             // ')' 
        correct = false ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      
      correct = true ;
      value = expressionValue ;
      return ; 
      
      
    }  // else if
    else if ( tokenType == Constant ) {               //              Constant
    
      if ( tokenValue[0] == 34 ) {         // "
        tokenValue.erase( 0, 1 ) ;
        tokenValue.erase( tokenValue.size()-1, 1 ) ;         // 將" 去掉 
        temp.value = tokenValue ;
        temp.type = STRING ;
      }  // if
      else if ( tokenValue[0] == 39 ) {    // '
        tokenValue.erase( 0, 1 ) ;
        tokenValue.erase( tokenValue.size()-1, 1 ) ;         // 將' 去掉 
        temp.value = tokenValue ;
        temp.type = CHAR ;
      } // else if
      else if ( tokenValue == "true" || tokenValue == "false" ) {
        temp.value = tokenValue ;
        temp.type = BOOL ;
      } // else if
      else {
        for ( int i = 0 ; i < tokenValue.size() ; i++ ) 
          if ( tokenValue[i] == '.' ) isFloat = true ;

        if ( isFloat == true && tokenValue[tokenValue.size()-1] == '.' ) {
          temp.value = tokenValue + "0" ; 
          temp.type = FLOAT ;
        } // if
        else if ( isFloat == true && tokenValue[0] == '.' ) {
          temp.value = "0" + tokenValue ; 
          temp.type = FLOAT ;
        } // else if
        else if ( isFloat == true && tokenValue[tokenValue.size()-1] != '.' && tokenValue[0] != '.' ) {
          temp.value = tokenValue ; 
          temp.type = FLOAT ;
        } // else if
        else {
          temp.value = tokenValue ;
          temp.type = INT ;
        } // else

      } // else 

      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      value = temp ;
      return ; 
    } // else if 
    else {
      correct = false ;
      return ;  
    } // else
    
    
    
    
  }  // Signed_unary_exp()
  
  void Unsigned_unary_exp( bool &correct, Formula &value ) {
  // unsigned_unary_exp
  // : Identifier [ '(' [ actual_parameter_list ] ')' | [ '[' expression ']' ] [ ( PP | MM ) ] ]
  // | Constant 
  // | '(' expression ')'   
  
    int tokenType ;
    string tokenValue ;
    bool actual_parameter_listCorrect = false ;
    Formula actual_parameter_listValue ;
    bool expressionCorrect = false ;
    Formula expressionValue ;
    Formula temp ;
    string idName ;
    bool isFloat = false ;
    int num ;
    double tempNUM ;
    stringstream sstream;
    
    PeekToken( tokenType, tokenValue ) ;
    
    
    if ( tokenType == Identifier ) {
      
      if ( CheckID( tokenValue ) == false ) {
        mNotDef = true ;
        correct = false ;
        return ;
      }  // if
      
      GetToken( tokenType, tokenValue ) ;
      idName = tokenValue ;                    // id
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType == LP ) {             // '('    
          
        GetToken( tokenType, tokenValue ) ;
          
        Actual_parameter_list( actual_parameter_listCorrect, actual_parameter_listValue ) ;
        
        PeekToken( tokenType, tokenValue ) ;
        
        if ( tokenType != RP ) {                      //    ')'
          correct = false ;
          return ;
        } // if
        
        GetToken( tokenType, tokenValue ) ;
        
        correct = true ;
        return ;
          
      } // if
      else if ( tokenType == LSB )  {             //  '['
      
        GetToken( tokenType, tokenValue ) ;
        
        Expression( expressionCorrect, expressionValue ) ;
        
        if ( expressionCorrect == false ) {
          correct = false ;
          return ;
        } // if
        
        PeekToken( tokenType, tokenValue ) ;
        
        if ( tokenType != RSB ) {              // ']'
          correct = false ;
          return ;
        } // if
        
        SearchArray( idName, expressionValue, temp ) ;             // 找出陣列的值 
        value = temp ;                                           // ++放後面是先輸出值 
        GetToken( tokenType, tokenValue ) ;
        
        PeekToken( tokenType, tokenValue ) ;
        
        if ( tokenType == PP ) {              // '++', '--'
          tempNUM = atof( value.value.c_str() ) ;
          tempNUM++ ;
          sstream << tempNUM ;
          temp.value = sstream.str() ;
          GetToken( tokenType, tokenValue ) ; 
          Assign_Var( idName, expressionValue, temp ) ;
        } // if
        else if ( tokenType == MM ) {        
          tempNUM = atof( value.value.c_str() ) ;
          tempNUM-- ;
          sstream << tempNUM ;
          temp.value = sstream.str() ;
          GetToken( tokenType, tokenValue ) ;
          Assign_Var( idName, expressionValue, temp ) ;
        } // if
        
        
        correct = true ;
        return ;
          
      } // else if
      else {

        SearchArray( idName, expressionValue, temp ) ;             // ID 的值 
        value = temp ;        
        PeekToken( tokenType, tokenValue ) ;
        
        if ( tokenType == PP ) {              // '++', '--'
          tempNUM = atof( temp.value.c_str() ) ;
          tempNUM++ ;
          sstream << tempNUM ;
          temp.value = sstream.str() ;
          GetToken( tokenType, tokenValue ) ;
          Assign_Var( idName, expressionValue, temp ) ;
        } // if
        else if ( tokenType == MM ) { 
          tempNUM = atof( temp.value.c_str() ) ;
          tempNUM-- ;
          sstream << tempNUM ;
          temp.value = sstream.str() ;
          GetToken( tokenType, tokenValue ) ;
          Assign_Var( idName, expressionValue, temp ) ;
        } // if
        
        
        correct = true ;
        return ;
      } // else
        
    } // if
    else if ( tokenType == LP ) {                          // '('
       
      GetToken( tokenType, tokenValue ) ;
      
      Expression( expressionCorrect, expressionValue ) ;
      
      if ( expressionCorrect == false ) {
        correct = false ;
        return ;
      } // if
      
      PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenType != RP ) {                             // ')' 
        correct = false ;
        return ;
      } // if
      
      GetToken( tokenType, tokenValue ) ;
      correct = true ;
      value = expressionValue ;
      return ; 
      
      
    }  // else if
    else if ( tokenType == Constant ) {    //                         Constant
    
      GetToken( tokenType, tokenValue ) ;
      
      if ( tokenValue[0] == 34 ) {         // "
        tokenValue.erase( 0, 1 ) ;
        tokenValue.erase( tokenValue.size()-1, 1 ) ;         // 將" 去掉 
        temp.value = tokenValue ;
        temp.type = STRING ;
      }  // if
      else if ( tokenValue[0] == 39 ) {    // '
        tokenValue.erase( 0, 1 ) ;
        tokenValue.erase( tokenValue.size()-1, 1 ) ;         // 將' 去掉 
        temp.value = tokenValue ;
        temp.type = CHAR ;
      } // else if
      else if ( tokenValue == "true" || tokenValue == "false" ) {
        temp.value = tokenValue ;
        temp.type = BOOL ;
      } // else if
      else {

        for ( int i = 0 ; i < tokenValue.size() ; i++ ) 
          if ( tokenValue[i] == '.' ) isFloat = true ;
        
        if ( isFloat == true && tokenValue[tokenValue.size()-1] == '.' ) {
          temp.value = tokenValue + "0" ;
          temp.type = FLOAT ;
        } // if
        else if ( isFloat == true && tokenValue[0] == '.' ) {
          temp.value = "0" + tokenValue ; 
          temp.type = FLOAT ;
        } // else if
        else if ( isFloat == true && tokenValue[tokenValue.size()-1] != '.' && tokenValue[0] != '.' ) {
          temp.value = tokenValue ; 
          temp.type = FLOAT ;
        } // else if
        else {
          temp.value = tokenValue ;
          temp.type = INT ;
        } // else

      } // else 
      
      correct = true ;
      value = temp ;
      return ; 
    } // else if 
    else {
      correct = false ;
      return ;  
    } // else
    
  
  } // Unsigned_unary_exp()
  
  
} ;



int main()  {
    
  Interpreter temp ;
    
  getchar() ;
  temp.Read() ;
  temp.Scanner() ;
  // temp.Print2() ;
  temp.Parsar() ;
    
    
} // main()
        
    


