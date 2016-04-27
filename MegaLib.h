#include <conio.h>
#include <stdio.h>
#include <malloc.h>
#include <string>
#include <vector>

class BigNat{
public:
	int len;
	unsigned long *d;//0-(256^4-1)

	~BigNat();
	BigNat();
	BigNat(unsigned long _a);
	BigNat(std::string _s);
	int to_str(char *buf, int buf_len);

	void PutHex();
	void Put();
	std::string ToString();

	const BigNat& operator= (const BigNat& _a)
	{
		if (this == &_a) return *this;

		free(d);
		if (_a.len != 0)
		{
			d = (unsigned long*)malloc(_a.len*sizeof(long));

			for (int i = 0; i < _a.len; i++)
				d[i] = _a.d[i];
		}
		else
			d = NULL;
		len = _a.len;
		return *this;
	}
	BigNat(const BigNat &_a);
	friend bool operator == (const BigNat& _a, const BigNat& _b);
	friend bool operator != (const BigNat& _a, const BigNat& _b);
	friend bool operator >(const BigNat& _a, const BigNat& _b);
	friend bool operator < (const BigNat& _a, const BigNat& _b);
	friend bool operator <= (const BigNat& _a, const BigNat& _b);
	friend bool operator >= (const BigNat& _a, const BigNat& _b);

	friend BigNat operator>>(const BigNat &_a, const unsigned long _b);
	friend BigNat operator+(const BigNat &_a, const BigNat &_b);
	friend BigNat operator-(const BigNat &_a, const BigNat &_b);
	friend BigNat operator*(const BigNat &_a, const BigNat &_b);
	friend BigNat operator/(const BigNat &_a, const BigNat &_b);
	friend BigNat operator%(const BigNat &_a, const BigNat &_b);

	void CutZero();
	int div_ten();

	void div_max(int i);
	void mul_max(int i);

};


class BigInt{
public:
	BigNat a;
	char sign;
	BigInt();
	BigInt(long _a);
	BigInt(std::string _s);
	BigInt(BigNat _a);
	void PutHex();
	void Put();
	std::string ToString();
	const BigInt& operator= (const BigInt& _a)
	{
		if (this == &_a) return *this;
		a = _a.a;
		sign = _a.sign;
		return *(this);
	}
	BigInt(const BigInt &_a);
	friend bool operator == (const BigInt& _a, const BigInt& _b);
	friend bool operator != (const BigInt& _a, const BigInt& _b);
	friend bool operator > (const BigInt& _a, const BigInt& _b);
	friend bool operator < (const BigInt& _a, const BigInt& _b);
	friend bool operator <= (const BigInt& _a, const BigInt& _b);
	friend bool operator >= (const BigInt& _a, const BigInt& _b);

	friend BigInt operator+(const BigInt &_a, const BigInt &_b);
	friend BigInt operator-(const BigInt &_a, const BigInt &_b);
	friend BigInt operator*(const BigInt &_a, const BigInt &_b);
	friend BigInt operator/(const BigInt &_a, const BigInt &_b);
	friend BigInt operator%(const BigInt &_a, const BigInt &_b);

	friend BigInt operator-(const BigInt &_a)
	{
		BigInt c = _a;
		c.sign = -c.sign;
		return c;
	}
};

class BigRat
{
	BigInt a;
	BigNat b;
public:

	BigRat()
	{
		a = 0;
		b = 1;
	}
	BigRat(long _a);
	BigRat(BigInt _a);
	BigRat(BigInt _a, BigNat _b);
	void Put();
	std::string ToString();

	friend bool operator == (const BigRat& _a, const BigRat& _b);
	friend bool operator != (const BigRat& _a, const BigRat& _b);
	friend bool operator >  (const BigRat& _a, const BigRat& _b);
	friend bool operator <  (const BigRat& _a, const BigRat& _b);
	friend bool operator <= (const BigRat& _a, const BigRat& _b);
	friend bool operator >= (const BigRat& _a, const BigRat& _b);

	friend BigRat operator+(const BigRat &_a, const BigRat &_b);
	friend BigRat operator-(const BigRat &_a, const BigRat &_b);
	friend BigRat operator*(const BigRat &_a, const BigRat &_b);
	friend BigRat operator/(const BigRat &_a, const BigRat &_b);
	friend BigRat operator%(const BigRat &_a, const BigRat &_b);

	friend BigRat operator-(const BigRat &_a)
	{
		BigRat c = _a;
		c.a.sign = -c.a.sign;
		return c;
	}

	void operator()(void);
};

class BigPol
{
private:
	std::vector<BigRat> d;
	void cut_zero();
public:
	~BigPol();
	BigPol();
	BigPol(std::string s);
	BigPol(const BigPol &_a);

	void Put();
	std::string ToString();

	friend bool operator == (const BigPol& _a, const BigPol& _b);
	friend bool operator != (const BigPol& _a, const BigPol& _b);
	friend bool operator >  (const BigPol& _a, const BigPol& _b);
	friend bool operator <  (const BigPol& _a, const BigPol& _b);
	friend bool operator <= (const BigPol& _a, const BigPol& _b);
	friend bool operator >= (const BigPol& _a, const BigPol& _b);

	friend BigPol operator+(const BigPol &_a, const BigPol &_b);
	friend BigPol operator-(const BigPol &_a, const BigPol &_b);
	friend BigPol operator*(const BigPol &_a, const BigPol &_b);
	friend BigPol operator/(const BigPol &_a, const BigPol &_b);
	friend BigPol operator%(const BigPol &_a, const BigPol &_b);

	friend BigPol operator-(const BigPol &_a)
	{
		int i;

		BigPol c = _a;

		for (i = 0; i < c.d.size();i++)
			c.d[i] = -c.d[i];
		return c;
	}
	void add_x(BigRat _a, int i);
	BigPol deg(BigPol _a);
};

BigNat pow(const BigNat _a, unsigned int _b);
BigNat powmod(const BigNat _a, const BigNat _b, const BigNat _mod);
BigNat NOD(BigNat _a, BigNat _b);
BigNat NOK(BigNat _a, BigNat _b);
BigPol POLY_NOD(BigPol _a, BigPol _b);
void diaf(BigInt a, BigInt b, BigInt c);

#ifndef MEGA_LIBC
#define MEGA_LIBC
#include "MegaLib.cpp"
#endif