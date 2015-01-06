#include<iostream>

using namespace std ;

class MyInt{
public:
	explicit MyInt(int v):value(v){}
	
	MyInt& operator++()
	{
		*this += 1;
		return *this;
	}
	
	MyInt operator++(int)
	{
		MyInt oldV = *this;
		*this += 1; 
		return oldV; 
	}
	
	MyInt& operator+=(int a)
	{
		value += a;
		return *this;
	}
	
	int getValue()
	{
		return value;
	}
	
private:
	int value ;

};

int main()
{
	MyInt i(1) ;
	i++++;
	cout<<i.getValue()<<endl;
	return 0;
}

