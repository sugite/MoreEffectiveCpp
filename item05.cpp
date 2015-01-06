#include<iostream>

using namespace std ;

template<typename T>
class Array{
public:
	//Array(int low , int high);
	Array(int size);

	T& operator[](int index);
	
	bool operator==(const Array<T>& rhs);
	
private:
	T *v_;
	int len ;
};

template<typename T>
Array<T>::Array(int size)
{
		v_ = new T[size];
		len = size ;
}

template<typename T>
T& Array<T>::operator[](int index)
{
	return v_[index];
}

template<typename T>
bool Array<T>::operator==(const Array<T>& rhs)
{
	if(len!=rhs.len) return false;
	return true;
}

int main()
{
	Array<int> a(10);
	Array<int> b(10);
	for(int i = 0 ; i < 10 ; ++i){
		a[i] = b[i] = i;
	}
	for(int i = 0 ; i < 10 ; ++i){
		if(a==b[i] )
			cout<<"equal"<<endl;
		else
			cout<<"no equal"<<endl;	
	}
	return 0;
}
