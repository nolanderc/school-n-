#pragma once
#include <cmath>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989

template<class T>
struct TypeVector2 {
	T x, y;

	TypeVector2() : x(0), y(0) {}
	TypeVector2(T s) : x(s), y(s) {}
	TypeVector2(T x, T y) : x(x), y(y) {}


	// Enkel vektoraddition
	TypeVector2<T> operator+(const TypeVector2<T>& other) const {
		return TypeVector2<T>(this->x + other.x, this->y + other.y);
	}

	// Enkel vektorsubtraktion
	TypeVector2<T> operator-(const TypeVector2<T>& other) const {
		return TypeVector2<T>(this->x - other.x, this->y - other.y);
	}

	// Enkel vektormultiplikation
	TypeVector2<T> operator*(const TypeVector2<T>& other) const {
		return TypeVector2<T>(this->x * other.x, this->y * other.y);
	}

	// Enkel vektorskalning
	TypeVector2<T> operator*(T scalar) const {
		return TypeVector2<T>(this->x * scalar, this->y * scalar);
	}

	// Enkel vektordivision
	TypeVector2<T> operator/(const TypeVector2<T>& other) const {
		return TypeVector2<T>(this->x / other.x, this->y / other.y);
	}

	// Enkel vektorskalning
	TypeVector2<T> operator/(T scalar) const {
		return TypeVector2<T>(this->x / scalar, this->y / scalar);
	}


	// En vektor i motsatt riktning
	TypeVector2<T> operator-() const {
		return TypeVector2<T>(-this->x, -this->y);
	}


	// Enkel ökning via addition
	void operator+=(const TypeVector2<T>& other) {
		this->x += other.x;
		this->y += other.y;
	}

	// Enkel minskning via subtraktion
	void operator-=(const TypeVector2<T>& other) {
		this->x -= other.x;
		this->y -= other.y;
	}

	// Enkel skalning via multiplikation
	void operator*=(const TypeVector2<T>& other) {
		this->x *= other.x;
		this->y *= other.y;
	}

	// Enkel skalning via division
	void operator/=(const TypeVector2<T>& other) {
		this->x /= other.x;
		this->y /= other.y;
	}

	// Enkel skalning via multiplikation
	void operator*=(T scalar) {
		this->x *= scalar;
		this->y *= scalar;
	}

	// Enkel skalning via division
	void operator/=(T scalar) {
		this->x /= scalar;
		this->y /= scalar;
	}



	// Avgör om två vektorer är desamma
	bool operator==(const TypeVector2<T>& other) {
		return this->x == other.x && this->y == other.y;
	}

	// Avgör om två vektorer inte är desamma
	bool operator!=(const TypeVector2<T>& other) {
		return this->x != other.x || this->y != other.y;
	}


	// Omvandla en typ av vektor till en annan
	template<class U>
	operator TypeVector2<U>() const {
		return TypeVector2<U>(U(this->x), U(this->y));
	}


	// Längden (absolutbeloppet) av en vektor
	double length() const {
		return sqrt(this->x*this->x + this->y*this->y);
	}

	// Returnerar en enhetsvektor med samma riktning som denna
	TypeVector2<T> normal() const {
		T len = this->length();
		return TypeVector2<T>(this->x / len, this->y / len);
	}

	// Dot multiplikation/projektion
	//
	// a.dot(b) = |a|*|b|*cos(v)
	// där a och b är vektorer, |a| och |b| är längden av dem och v är vinkeln mellan dem 
	T dot(const TypeVector2<T>& other) const {
		return this->x * other.x + this->y * other.y;
	}

	// Returnerar en vektor som är vinkelrät till denna, roterad medsols
	TypeVector2<T> perpCW() const {
		return TypeVector2<T>(this->y, -this->x);
	}

	// Returnerar en vektor som är vinkelrät till denna, roterad motsols
	TypeVector2<T> perpCCW() const {
		return TypeVector2<T>(-this->y, this->x);
	}


	// Skapar en vektor som är roterad ett antal grader, motsols
	// https://en.wikipedia.org/wiki/Rotation_(mathematics)
	static TypeVector2<T> rotated(double angle, TypeVector2<T> v = { 1, 0 }) {
		double cos0 = cos(PI * angle / 180.0);
		double sin0 = sin(PI * angle / 180.0);

		return {
			v.x * cos0 - v.y * sin0,
			-v.x * sin0 - v.y * cos0,
		};
	}


	// Reflekterar en vektor runt en normal
	// https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	TypeVector2<T> reflect(TypeVector2<T> normal) {
		return *this - 2 * (*this * normal) * normal;
	}
};


// För multiplikation med en skalär innan vektorn
template<class T>
TypeVector2<T> operator*(double scalar, const TypeVector2<T>& vec) {
	return TypeVector2<T>(vec.x * scalar, vec.y * scalar);
}


// Förbestämda typer av vektorer
typedef TypeVector2<double> Vector2;
typedef TypeVector2<int> Vector2i;
