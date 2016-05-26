#include "color.h"
#include <iostream>

Color operator + ( const Color& A , const Color& B ) {
	return Color( A.r + B.r , A.g + B.g , A.b + B.b );
}

Color operator - ( const Color& A , const Color& B ) {
	return Color( A.r - B.r , A.g - B.g , A.b - B.b );
}

Color operator * ( const Color& A , const Color& B ) {
	return Color( A.r * B.r , A.g * B.g , A.b * B.b );
}

Color operator * ( const Color& A , const double& k ) {
	return Color( A.r * k , A.g * k , A.b * k );
}

Color operator / ( const Color& A , const double& k ) {
	return Color( A.r / k , A.g / k , A.b / k );
}

Color operator * (  const double& k, const Color& A ) {
	return Color( A.r * k , A.g * k , A.b * k );
}

Color operator / (  const double& k ,const Color& A ) {
	return Color( A.r / k , A.g / k , A.b / k );
}


Color& operator += ( Color& A , const Color& B ) {
	A = A + B;
	return A;
}

Color& operator -= ( Color& A , const Color& B ) {
	A = A - B;
	return A;
}

Color& operator *= ( Color& A , const double& k ) {
	A = A * k;
	return A;
}

Color& operator /= ( Color& A , const double& k ) {
	A = A / k;
	return A;
}

void Color::Confine() {
	if ( r > 1 ) r = 1;
	if ( g > 1 ) g = 1;
	if ( b > 1 ) b = 1;
}
Color Color::Exp(){
	return Color(exp(r),exp(g),exp(b));
}
std::istream& operator >> (std::istream& fin,  Color& A){
	return (fin >> A.r >> A.g >> A.b);
}

