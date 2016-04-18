#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <cmath>
class Color {
public:
	double r , g , b;

	Color( double R = 0 , double G = 0 , double B = 0 ) : r( R ) , g( G ) , b( B ) {}
	~Color() {}
	friend Color operator + ( const Color& , const Color& );
	friend Color operator - ( const Color& , const Color& );
	friend Color operator * ( const Color& , const Color& );
	friend Color operator * ( const Color& , const double& );
	friend Color operator / ( const Color& , const double& );
	friend Color operator * ( const double&, const Color&);
	friend Color operator / ( const double&, const Color&);
	friend Color& operator += ( Color& , const Color& );
	friend Color& operator -= ( Color& , const Color& );
	friend Color& operator *= ( Color& , const double& );
	friend Color& operator /= ( Color& , const double& );
	Color Exp();
	void Confine();
	friend std::istream& operator >> (std::istream& , Color&);
};

#endif
