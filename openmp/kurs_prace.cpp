#include <cstring>
#include <iostream>

class Text
 {
  public :
    
    Text()
     : size_(0),
       data_(0)
     {}
    
    Text( char const * str )
     : size_(std::strlen(str)),
       data_(size_?(new char [size_+1]):0)
     { 
        if (size_) std::copy(str,str+size_+1,data_) ; 
        std::cout << "custom ctor" << std::endl;
     }
    
    Text( Text const & t )
     : size_(t.size_),
       data_(size_?(new char [size_+1]):0)
     {
      std::cout<<"copy constructor"<<std::endl ;
      if (size_) std::copy(t.data_,t.data_+size_+1,data_) ;
     }
    
    Text & operator=( Text const & t )
     {
      std::cout<<"copy assignment"<<std::endl ;
      if (this == &t) return *this ;
      delete [] data_ ;
      size_ = t.size_ ;
      data_ = size_?(new char [size_+1]):0 ;
      if (size_) std::copy(t.data_,t.data_+size_+1,data_) ;
      return *this ;
     }
    Text(Text&& t) : size_(t.size_), data_(t.data_) // is it correct?
    {
        std::cout << "move ctor" <<std::endl;
        t.size_ = 0;
        t.data_ = nullptr;
    }
    
    Text & operator=(Text&& t) // is it correct?
    {
        std::cout << "move assignment" <<std::endl;
        delete [] data_;
        size_ = t.size_;
        data_ = t.data_;
        t.size_ = 0;
        t.data_ = nullptr;
        return *this;
    }
    
    unsigned int size() const { return size_; }
    
    ~Text()
     { delete [] data_ ; }
    
    unsigned int taille()
     { return size_ ; }
    
    char & operator[]( unsigned int i )
     { return data_[i] ; }
    
    friend std::ostream & operator<<( std::ostream & os, Text const & t )
     { if (t.size_) return os<<t.data_ ; else return os ; }
    
  private :
    
    unsigned int size_ ;
    char * data_ ;
 } ;
 
Text hello()
 { return "hello" ; }

Text uppercase( Text t )
 {
  std::cout << " foo " << std::endl;
  for ( unsigned int i=0 ; i<t.taille() ; ++i )
   { t[i] = std::toupper(t[i]) ; }
  std::cout << " bar " << std::endl;
  return t ;
 }

int main()
 {
    Text h ("hello!!");
//   std::cout<<uppercase(hello())<<std::endl ; // hello() returns rvalue, so move constructor works fine
  std::cout<<uppercase(h)<<std::endl ; // h is an lvalue
//   std::cout<<uppercase(std::move(h))<<std::endl ; // we avoid copy, but h becomes EMPTY
//   std::cout << "h is " << h << " and has " << h.size() << " letters" << std::endl;
  
  return 0 ;
 }
 
/*
My question:
 
I am working on the exercise from move constuctors (2-IndividualWork->Refs&Pointers-en.1-move).
I implemented move constuctor / assignment operator, but I'm not sure is it correct. Can someone please verify?
It's quite possible there is a mistake, because when I define separate object in main (see [1]), I get a call to both move AND copy constructor... Can we look into this?

David's answer:
Hi. You trigger the copy constructor because you replaced uppercase(hello()) with Text h("hello!!") ; uppercase(h) .
In the first case, the object returned by hello()  is a temporary object, i.e. a right value, and when giving this object to uppercase, the compiler select the move constructor so to construct uppercase argument.
In the second case, h is a stable object staying in a variable, associate dto a memory slot, i.e. a left value. When about to prepare the argument t for uppercase , the compiler will prefer the copy constructor, because it cannot link the && move constructor &&t  to the left value h.
Actually, I was puzzled by the presence of both the copy and move display in your case, and there is more to say.
The move constructor is not used where I though : the compiler take profit of it for the return of the uppercase function. For what concerns the input argument of uppercase when you use uppercase(hello()), the compiler is clever enough so to apply the Return Value Optimization (RVO) for hello(), and directly construct uppercase input t from the value "hello" in the hello return statement.
By the way, you may suppress the copy construction with  h("hello!!") ; uppercase(std::move(h)) , but afterwards your h will be empty, which is generally not what one wants wjen he creates such a variable.


--- About RVO ---
Question:
But now I would like to ask you about RVO, becouse I thought that it works by implicitly switching code:

Text uppercase(Text t)
{
...
return t;
}

into something like that:

void uppercase(Text t, Text& t_ret)
{
...
t_ret = t
}

and no copy / move is performed at all; just c - style side effect on the input argument. Now it looks like I was wrong, and there is more to it, isn't it?

Strictly syntaxically speaking, you cannot use such a Text & t_ret  equivalent if the caller of the function does not store the result in a variable (lvalue) but put it in a temporary object which is immediatly reused thus deleted. I would rather think of RVO as some some sort of inlining, where the compiler directly construct the calling code object from the values which are returned. I am not sure I am clear, but it i snot 100% in my mind also ;)
*/
