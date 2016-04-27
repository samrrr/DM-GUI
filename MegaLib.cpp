//using namespace std;

BigNat::~BigNat()
{
	free(d);
}
BigNat::BigNat()
{
	len = 0;
	d = NULL;
}
BigNat::BigNat(unsigned long _a)
{
	if (_a != 0)
	{
		len = 1;
		d = (unsigned long*)malloc(len*sizeof(long));
		d[0] = _a;
	}
	else
	{
		len = 0;
		d = NULL;
	}
}

BigNat::BigNat(std::string _s)
{
	len = 0;
	d = NULL;
	const char *s = _s.data();
	int i = 0;
	BigNat c;
	if (s[i] == '-')
	{
		*this = 0;
		return;
	}

	while (i<_s.length())
	{
		if (s[i] < '0' || s[i] > '9')
		{
			*this = 0;
			return;
		}

		c = c * 10 + (s[i] - '0');
		i++;
	}

	*this=c;
}

int BigNat::to_str(char *buf, int buf_len)
{
	if (len * 4 * 3 > buf_len)
		return -1;

	char*g;
	int slen = len * 4;

	g = (char*)d;

	for (int i = 0; i < slen; i++)
	{
		buf[i * 3 + 0] = g[i] & 15 + '0';
		buf[i * 3 + 1] = (g[i] & 240) / 16 + '0';
		buf[i * 3 + 2] = ' ';
	}
	buf[len * 3 + 2] = 0;
	return 0;
}

char HexChar(int _i)
{
	if (_i >= 0 && _i <= 9)
		return '0' + _i;

	if (_i >= 10 && _i <= 15)
		return 'A' + (_i - 10);

	return '!';
}


void BigNat::Put()
{
	BigNat a;
	int i;

	std::string s;

	s = "";

	a = *this;

	do
	{
		i=a.div_ten();
		s = (char)(i+'0')+s;
	} while (a != 0);
	printf("%s",s.data());

}

std::string BigNat::ToString()
{
	BigNat a;
	int i;

	std::string s;

	s = "";

	a = *this;

	do
	{
		i = a.div_ten();
		s = (char)(i + '0') + s;
	} while (a != 0);
	return s;
}

void BigNat::PutHex()
{
	unsigned long r, o, j;
	int i;

	o = ((1 << 4) - 1) << 28;

	for (i = len - 1; i >= 0; i--)
	{
		j = d[i];
		for (r = 0; r < 8; r++)
		{
			printf("%c", HexChar((j & o) >> 28));
			j=j << 4;
		}
		printf(" ");
	}
}

/*
const BigNat& operator= (const BigNat& _a)
{
	if (this == &_a) return *this;

	free(d);
	d = (unsigned long*)malloc(_a.len*sizeof(long));

	for (int i = 0; i < _a.len; i++)
		d[i] = _a.d[i];
	len = _a.len;
}
*/

BigNat::BigNat(const BigNat &_a)
{
	len = _a.len;
	if (len == 0)
	{
		d = NULL;
		return;
	}
	//printf("Cre: %i\n", DEtest);
	d = (unsigned long*)malloc(len*sizeof(long));
	for (int i = 0; i < len; i++)
	{
		d[i] = _a.d[i];
	}

}
bool operator == (const BigNat& _a, const BigNat& _b)
{
	BigNat c;

	if (_a.len != _b.len)
		return 0;

	if (_a.len == 0)
	if (_b.len == 0)
		return 1;
	else
		return 0;

	if (_a.len == 0)
		return 0;
	for (int i = 0; i < _a.len; i++)
	if (_a.d[i] != _b.d[i])
		return 0;

	return 1;
}

bool operator != (const BigNat& _a, const BigNat& _b)
{
	return !(_a == _b);
}

bool operator >(const BigNat& _a, const BigNat& _b)
{
	BigNat c;

	if (_a.len > _b.len)
		return 1;
	if (_a.len < _b.len)
		return 0;

	if (_a.len == 0)
	if (_b.len == 0)
		return 0;

	for (int i = _a.len - 1; i >= 0; i--)
	if (_a.d[i] != _b.d[i])
		return _a.d[i] > _b.d[i];

	return 0;
}

bool operator < (const BigNat& _a, const BigNat& _b)
{
	BigNat c;

	if (_a.len > _b.len)
		return 0;
	if (_a.len < _b.len)
		return 1;

	if (_a.len == 0)
	if (_b.len == 0)
		return 0;

	for (int i = _a.len - 1; i >= 0; i--)
	if (_a.d[i] != _b.d[i])
		return _a.d[i] < _b.d[i];

	return 0;
}

bool operator <= (const BigNat& _a, const BigNat& _b)
{
	return !(_a > _b);
}

bool operator >= (const BigNat& _a, const BigNat& _b)
{
	return !(_a < _b);
}

BigNat operator>>(const BigNat &_a, const unsigned long _b)
{
	int i;
	BigNat c;
	if (_a.len == 0)
		return c;

	unsigned long r,o;

	r = _b % 32;

	o = ((1 << _b) - 1);

	c.len = _a.len;
	c.d = (unsigned long*)malloc(sizeof(long)*(c.len));
	c.d[0] = _a.d[0] >> r;
	for (i = 1; i<_a.len; i++)
	{
		c.d[i] = _a.d[i] >> r;
		c.d[i - 1] |= (_a.d[i] & o) << (32 - r);
	}
	c.CutZero();
	return c;
}

BigNat operator+(const BigNat &_a, const BigNat &_b)
{
	if (_b.len > _a.len)
		return _b + _a;

	BigNat c;
	c.len = (_a.len > _b.len ? _a.len : _b.len) + 1;
	c.d = (unsigned long*)malloc(sizeof(long)*c.len);
	int i;

	for (i = 0; i < _a.len; i++)
		c.d[i] = _a.d[i];
	for (; i < c.len; i++)
		c.d[i] = 0;

	int per;
	unsigned long s1, s2;
	per = 0;

	for (i = 0; i < _b.len; i++)
	{
		s1 = _a.d[i] + _b.d[i];
		s2 = s1 + per;
		per = 0;
		if (s1 < _a.d[i])
		{
			per = 1;
		}
		else
		if (s2 < s1)
		{
			per = 1;
		}
		c.d[i] = s2;
	}

	for (; i < _a.len; i++)
	{
		s1 = _a.d[i] + per;
		per = 0;
		if (s1 < _a.d[i])
		{
			per = 1;
		}
		c.d[i] = s1;
	}

	c.d[i] = per;
	c.CutZero();
	return c;
}

BigNat operator-(const BigNat &_a, const BigNat &_b)
{
	if (_a < _b)
		return 0;

	BigNat c;
	c.len = _a.len;
	c.d = (unsigned long*)malloc(sizeof(long)*c.len);
	int i, r, o, j;

	for (i = 0; i < _a.len; i++)
		c.d[i] = _a.d[i];

	for (; i < c.len; i++)
		c.d[i] = 0;

	int per;
	unsigned long s1, s2;
	per = 0;

	for (i = 0; i < _b.len; i++)
	{
		s1 = _a.d[i] - _b.d[i];
		s2 = s1 - per;
		per = 0;
		if (s1 > _a.d[i])
		{
			per = 1;
		}
		else
		if (s2 > s1)
		{
			per = 1;
		}
		c.d[i] = s2;
	}

	for (; i < _a.len; i++)
	{
		s1 = _a.d[i] - per;
		per = 0;
		if (s1 > _a.d[i])
		{
			per = 1;
		}
		c.d[i] = s1;
	}

	c.CutZero();
	return c;
}
BigNat operator*(const BigNat &_a, const BigNat &_b)
{
	int i;
	if (_b == 2)
	{
		BigNat c;
		c.len = _a.len + 1;
		c.d = (unsigned long*)malloc(sizeof(long)*(c.len));
		c.d[c.len - 1] = 0;
		for (i = _a.len - 1; i >= 0; i--)
		{
			c.d[i] = _a.d[i] << 1;
			c.d[i + 1] |= (_a.d[i] & 2147483648) ? 1 : 0;
		}
		c.CutZero();
		return c;
	}

	if (_b > _a)
		return _b*_a;

	BigNat a, b, c;

	a = _a;
	b = _b;
	c = 0;

	do
	{
		if (b % 2 == 1)
		{
			c = c + a;
		}
		a = a * 2;
		b = b / 2;
	} while (b>0);
	return c;
}
BigNat operator/(const BigNat &_a, const BigNat &_b)
{
	if (_a == 0)
		return 0;
	if (_b == 0)
		return 0;

	if (_b == 2)
	{
		int i;
		BigNat c;
		if (_a.len == 0)
			return c;
		c.len = _a.len;
		c.d = (unsigned long*)malloc(sizeof(long)*(c.len));
		c.d[0] = _a.d[0] >> 1;
		for (i = 1; i<_a.len; i++)
		{
			c.d[i] = _a.d[i] >> 1;
			c.d[i - 1] |= (_a.d[i] & 1) ? 2147483648 : 0;
		}
		c.CutZero();
		return c;
	}

	BigNat c, p, f, m;
	int i, r, o, j;

	f = _b;
	p = _a;
	c = 0;
	m = 1;
	do{
		m = m * 2;
		f = f * 2;
	} while (!(f >p));
	m = m / 2;
	f = f / 2;
	do
	{
		if (p > f)
		{
			c = c + m;
			p = p - f;
		}
		else
		if (p == f)
		{
			c = c + m;
			m = 1;
		}
		m = m / 2;
		f = f / 2;
	} while (m > 0);

	c.CutZero();
	return c;
}
BigNat operator%(const BigNat &_a, const BigNat &_b)
{
	if (_b == 0)
		return 0;
	if (_b == 2)
	{
		BigNat c;
		if (_a.len == 0)
			return c;
		c = _a.d[0] % 2;
		return c;
	}
	BigNat p, f, m;


	f = _b;
	p = _a;
	m = 1;
	do{
		m = m * 2;
		f = f * 2;
	} while (f <= p);
	m = m / 2;
	f = f / 2;
	do
	{
		if (p >= f)
		{
			p = p - f;
		}
		m = m / 2;
		f = f / 2;
	} while (m > 0);

	return p;

}


void BigNat::CutZero()
{
	while (len>0 && d[len - 1] == 0)
	{
		d = (unsigned long*)realloc(d, sizeof(long)*len);
		len--;
	}
}

int BigNat::div_ten()
{
	//0.0001100110011001100
	BigNat a,b,c;

	c = *this;

	c = (c >> 4) + (c >> 5);
	c = c + (c >> 4);
	c = c + (c >> 8);
	c = c + (c >> 16);

	int i = 1;

	while (i < c.len*2+1)
	{
		a = c;
		a.div_max(i);
		c = c + a;
		i = i * 2;
	}

	b = (*this - c * 10);

	/*
	(*this).PutHex();
	puts("");
	(c * 10).PutHex();
	puts("");
	b.PutHex();
	puts("");
	*/

	*this = c + b / 10;

	b = b % 10;
	if (b.len > 0)
		return b.d[0];

	return 0;
}

void BigNat::div_max(int _i)
{
	if (_i >= len)
	{
		*this = 0;
		return;
	}
	int i,r;
	
	r = len - _i;
	
	for (i = 0; i < r; i++)
		d[i] = d[i + _i];

	len = len - _i;
}

void BigNat::mul_max(int _i)
{
	if (*this == 0)
		return;

	int i,r;

	r = len;
	len = len + _i;

	for (i = r-1; i >= 0; i--)
		d[i + _i] = d[i];
	for (i = 0; i < _i; i++)
		d[i] = 0;
}



BigInt::BigInt()
{
	(*this).a = 0;
	(*this).sign = 1;
}

BigInt::BigInt(long _a)
{
	if (_a >= 0)
	{
		a = _a;
		sign = 1;
	}
	else
	{
		a = 4294967295-(unsigned long)_a+1;
		sign = -1;
	}
}

BigInt::BigInt(std::string _s)
{
	std::string s = "";
	if (_s.data()[0] == '-')
	{
		for (int i = 1; i < _s.length(); i++)
			s = s + _s.data()[i];
		a = s;
		sign = -1;
	}
	else
	{
		a = _s;
		sign = 1;
	}
}

BigInt::BigInt(BigNat _a)
{
	(*this).a = _a;
	(*this).sign = 1;
}

void BigInt::PutHex()
{

}

void BigInt::Put()
{
	if (sign == -1)
		printf("-");
	a.Put();
}

std::string BigInt::ToString()
{
	std::string s;
	s = a.ToString();
	if (sign == -1)
		s = '-' + s;
	
	return s;
}

BigInt::BigInt(const BigInt &_a)
{
	a = _a.a;
	sign = _a.sign;
}

bool operator == (const BigInt& _a, const BigInt& _b)
{
	if (_a.a == 0)
		return _b.a == 0;
	if (_b.a == 0)
		return 0;

	if (_a.sign != _b.sign)
		return 0;

	return _a.a == _b.a;
}

bool operator != (const BigInt& _a, const BigInt& _b)
{
	return !(_a == _b);
}

bool operator > (const BigInt& _a, const BigInt& _b)
{
	if (_a.a == 0)
		return _b.sign == -1 && _b.a != 0;
	if (_b.a == 0)
		return _a.sign == 1;

	if (_a.sign != _b.sign)
		return _a.sign==1;

	if (_a.sign == 1)
		return _a.a > _b.a;
	return _b.a > _a.a;
}

bool operator < (const BigInt& _a, const BigInt& _b)
{
	return _b > _a;
}

bool operator <= (const BigInt& _a, const BigInt& _b)
{
	return !(_a > _b);
}

bool operator >= (const BigInt& _a, const BigInt& _b)
{
	return !(_a < _b);
}

BigInt operator+(const BigInt &_a, const BigInt &_b)
{
	BigInt c;
	if (_a.sign != _b.sign)
	{
		if (_a.a>_b.a)
		{
			c = _a.a - _b.a;
			c.sign = _a.sign;
		}
		else
		{
			c = _b.a - _a.a;
			c.sign = _b.sign;
		}
	}
	else
	{
		c = _b.a + _a.a;
		c.sign = _b.sign;
	}
	return c;
}

BigInt operator-(const BigInt &_a, const BigInt &_b)
{
	return _a + (-_b);
}

BigInt operator*(const BigInt &_a, const BigInt &_b)
{
	BigInt c;
	c.a = _a.a *_b.a;
	c.sign = _a.sign *_b.sign;
	return c;
}

BigInt operator/(const BigInt &_a, const BigInt &_b)
{
	BigInt c;
	c.a = _a.a /_b.a;
	c.sign = _a.sign *_b.sign;
	return c;
}

BigInt operator%(const BigInt &_a, const BigInt &_b)
{
	BigInt c;
	c.a = _a.a % _b.a;
	c.sign = _a.sign;
	if (c < 0)
		c = c + _b;
	return c;
}

BigRat::BigRat(BigInt _a, BigNat _b)
{
	if (_b == 0)
	{
		a = 1;
		b = 1;
		return;
	}
	a = _a;
	b = _b;
	BigNat c;
	c=NOD(a.a,b);
	a = a / c;
	b = b / c;
}
BigRat::BigRat(BigInt _a)
{
	a = _a;
	b = 1;
}

BigRat::BigRat(long _a)
{
	a = _a;
	b = 1;
}

void BigRat::Put()
{
	a.Put();
	if (b != 1)
	{
		printf("/");
		b.Put();
	}
}
std::string BigRat::ToString()
{
	std::string s;
	s=a.ToString();
	if (b != 1)
	{
		s = s + '/';
		s=s+b.ToString();
	}
	return s;
}

bool operator == (const BigRat& _a, const BigRat& _b)
{
	return _a.a == _b.a && _a.b == _b.b;
}

bool operator != (const BigRat& _a, const BigRat& _b)
{
	return !(_a == _b);
}

bool operator >(const BigRat& _a, const BigRat& _b)
{
	return _a.a*_b.b-_a.b*_b.a > 0;
}

bool operator < (const BigRat& _a, const BigRat& _b)
{
	return _b > _a;
}

bool operator <= (const BigRat& _a, const BigRat& _b)
{
	return !(_a > _b);
}

bool operator >= (const BigRat& _a, const BigRat& _b)
{
	return !(_a < _b);
}

BigRat operator+(const BigRat &_a, const BigRat &_b)
{
	BigInt a0, a1;
	BigNat b0, b1;
	BigNat b00, b10;
	BigNat c;
	BigRat res;

	a0 = _a.a;
	a1 = _b.a;

	b0 = _a.b;
	b1 = _b.b;

	c = NOD(b0,b1);

	b00 = b0 / c;
	b10 = b1 / c;

	res.a = a0*b10+a1*b00;

	res.b = b0 * b10;

	c = NOD(res.a.a, res.b);

	res.a = res.a / c;

	res.b = res.b / c;
	
	return res;
}

BigRat operator-(const BigRat &_a, const BigRat &_b)
{
	return _a + (-_b);
}

BigRat operator*(const BigRat &_a, const BigRat &_b)
{
	BigRat a = _a;
	BigRat b = _b;
	BigNat c, d;
	BigRat res;

	c = NOD(a.a.a, b.b);
	d = NOD(b.a.a, a.b);

	a.a.a = a.a.a / c;
	b.b = b.b / c;

	b.a.a = b.a.a / d;
	a.b = a.b / d;

	res.a = a.a.a * b.a.a;
	res.b = a.b * b.b;
	res.a.sign = a.a.sign * b.a.sign;

	return res;
}

BigRat operator/(const BigRat &_a, const BigRat &_b)
{
	BigRat a = _a;
	BigRat b = _b;
	BigNat c, d;
	BigRat res;

	c = NOD(a.a.a, b.a.a);
	d = NOD(b.b, a.b);

	a.a.a = a.a.a / c;
	b.a.a = b.a.a / c;

	b.b = b.b / d;
	a.b = a.b / d;

	res.a = a.a.a * b.b;
	res.b = a.b * b.a.a;
	res.a.sign = a.a.sign * b.a.sign;

	return res;
}

BigRat operator%(const BigRat &_a, const BigRat &_b)
{
	return BigRat();
}

void BigRat::operator()()
{
	a = 0;
	b = 0;
}


BigPol::BigPol()
{
	d = std::vector<BigRat>(0);
}

bool is_number(char ch)
{
	return ch >= '0' && ch <= '9';
}
BigPol::~BigPol()
{
	int i;

	for (i = 0; i < d.size(); i++)
		d[i] = 0;
}

BigPol::BigPol(std::string _s)
{
	std::string s;
	if (_s.data()[0]=='-')
		s = _s.data();
	else
	{
		s = _s.data();
		s = '+' + s;
	}

	int i,r;
	BigInt a;
	BigNat b;
	BigRat c;

	d = std::vector<BigRat>(0);

	if (_s == "")
	{
		return;
	}

	i = 0;

	do{
		if (s[i] == '-')
			r = -1;
		else
			r = 1;

		i++;

		if (s[i] != 'x')
		{
			a = 0;
			do
			{
				a = a * 10 + (s[i] - '0');
				i++;
			} while (s[i] != 'x' && s[i] != '/' && s[i] != '+' && s[i] != '-' && s[i] != 0);
			a = a * r;
		}
		else
			a = 1 * r;

		if (s[i] == '/')
		{
			i++;
			b = 0;
			do
			{
				b = b * 10 + (s[i] - '0');
				i++;
			} while (s[i] != 'x' && s[i] != '+' && s[i] != '-' && s[i] != 0);
		}
		else
			b = 1;

		c = BigRat(a, b);

		if (s[i] == 'x')
		{

			i++;

			if (s[i] != '+' && s[i] != '-' && s[i] != NULL)
			{
				r = 0;
				do
				{
					r = r * 10 + (s[i] - '0');
					i++;
				} while (is_number(s[i]));
			}
			else
				r = 1;


			(*this).add_x(c,r);
		}
		else
		{
			(*this).add_x(c,0);
		}

	} while (s[i]!=0);

}

void BigPol::add_x(BigRat _a, int _i)
{
	if (_i < 0)
		return;

	if (_i >= d.size())
	{
		int i;

		d.resize(_i+1);

	}

	d[_i] = d[_i] + _a;
}

BigPol::BigPol(const BigPol &_a)
{
	int i;

	d = _a.d;
}

void BigPol::Put()
{
	int i;
	
	i = d.size()-1;
	if (i < 0)
	{
		printf("0");
		return;
	}

	if (i>0 && (d[i] == 1 || d[i] == -1))
	{
		if (d[i] == -1)
			printf("-");
	}
	else
		d[i].Put();
	if (i > 0)
	{
		printf("x");
		if (i > 1)
			printf("%i", i);
	}

	i--;

	while (i>=0)
	{
		if (d[i] != 0)
		{
			if (d[i] > 0)
				printf("+");
			if (i>0 && (d[i] == 1 || d[i] == -1))
			{
				if (d[i] == -1)
					printf("-");
			}
			else
				d[i].Put();
			if (i > 0)
			{
				printf("x");
				if (i > 1)
					printf("%i",i);
			}
		}
		i--;
	}

}

std::string BigPol::ToString()
{
	std::string s;
	int i;

	i = d.size() - 1;
	if (i < 0)
	{
		s = "0";
		return s;
	}


	if (i>0 && (d[i] == 1 || d[i] == -1))
	{
		if (d[i] == -1)
			s="-";
	}
	else
		s=d[i].ToString();
	if (i > 0)
	{
		s=s+"x";
		if (i > 1)
			s=s+((BigNat)i).ToString();
	}

	i--;

	while (i >= 0)
	{
		if (d[i] != 0)
		{
			if (d[i] > 0)
				s = s + "+";
			if (i>0 && (d[i] == 1 || d[i] == -1))
			{
				if (d[i] == -1)
					s = s + "-";
			}
			else
				s=s+d[i].ToString();
			if (i > 0)
			{
				s = s + "x";
				if (i > 1)
					s = s + ((BigNat)i).ToString();
			}
		}
		i--;
	}
	return s;
}

void BigPol::cut_zero()
{
	while (d.size() > 0 && d[d.size() - 1] == 0)
	{
		d.resize(d.size() - 1);
	}
}

bool operator == (const BigPol& _a, const BigPol& _b)
{
	if (_b.d.size() == 0)
		return (_a.d.size() == 0 || _a.d.size() == 1 && _a.d[0] == 0);
	if (_a.d.size() == 0)
		return (_b.d.size() == 0 || _b.d.size() == 1 && _b.d[0] == 0);

	if (_a.d.size() != _b.d.size())
		return 0;

	int i;

	for (i = _a.d.size() - 1; i >= 0; i--)
		if (_a.d[i] != _b.d[i])
			return 0;

	return 1;
}
bool operator != (const BigPol& _a, const BigPol& _b)
{
	return !(_a == _b);
}
bool operator >  (const BigPol& _a, const BigPol& _b)
{
	if (_a.d.size() != _b.d.size())
		return _a.d.size() > _b.d.size();

	int i;

	for (i = _a.d.size() - 1; i >= 0; i--)
	if (_a.d[i] != _b.d[i])
		return _a.d[i]>_b.d[i];

	return 0;
}
bool operator <  (const BigPol& _a, const BigPol& _b)
{
	return _b > _a;
}
bool operator <= (const BigPol& _a, const BigPol& _b)
{
	return !(_a > _b);
}
bool operator >= (const BigPol& _a, const BigPol& _b)
{
	return !(_a < _b);
}

BigPol operator+(const BigPol &_a, const BigPol &_b)
{
	if (_a.d.size()>_b.d.size())
		return _b + _a;

	BigPol c;
	int i, r;

	c = _b;
	r = _a.d.size();

	for (i = 0; i < r; i++)
	{
		c.d[i] = c.d[i] + _a.d[i];
	}

	c.cut_zero();

	return c;
}
BigPol operator-(const BigPol &_a, const BigPol &_b)
{
	if (_a.d.size()>_b.d.size())
		return _b - _a;

	BigPol c;
	int i, r;

	c = _b;
	r = _a.d.size();

	for (i = 0; i < r; i++)
	{
		c.d[i] = c.d[i] - _a.d[i];
	}

	c.cut_zero();

	return c;
}

BigPol operator*(const BigPol &_a, const BigPol &_b)
{
	int i, r,a_max,b_max;
	BigPol c;

	a_max = _a.d.size();
	b_max = _b.d.size();
	for (i = 0; i < a_max; i++)
	if (_a.d[i] != 0)
	for (r = 0; r < b_max; r++)
	if (_b.d[r] != 0)
		c.add_x(_a.d[i]*_b.d[r],i+r);

	c.cut_zero();

	return c;
}

BigPol operator/(const BigPol &_a, const BigPol &_b)
{
	if (_b == (std::string)"0")
		return (std::string)"0";

	BigPol c,res;
	int ste;
	int i,r;

	c = _a;
	res = (std::string)"0";
	ste = _a.d.size() - _b.d.size();

	while (ste >= 0)
	{
		r = _b.d.size();
		res.add_x(c.d[r - 1 + ste] / _b.d[r - 1],  ste);
		for (i = 0; i < r; i++)
			c.d[i + ste] = c.d[i + ste] - _b.d[i] * c.d[r-1 + ste] / _b.d[r - 1];
		//c[r-1+ste] = 0;
		ste--;
	}

	res.cut_zero();

	return res;
}

BigPol operator%(const BigPol &_a, const BigPol &_b)
{
	if (_b == (std::string)"0")
		return (std::string)"0";

	BigPol c, res;
	int ste;
	int i, r;

	c = _a;
	res = (std::string)"0";
	ste = _a.d.size() - _b.d.size();

	while (ste >= 0)
	{
		r = _b.d.size();
		res.add_x(c.d[r - 1 + ste] / _b.d[r - 1], ste);
		for (i = 0; i < r; i++)
			c.d[i + ste] = c.d[i + ste] - _b.d[i] * c.d[r - 1 + ste] / _b.d[r - 1];
		//c[r-1+ste] = 0;
		ste--;
	}

	c.cut_zero();

	return c;
}






BigNat pow(const BigNat _a, unsigned int _b)
{
	if (_b == 0)
		return !(_a == 0);
	if (_b < 0)
		return 0;
	BigNat c, d, m;
	d = _b;
	c = 1;
	m = _a;
	if (d % 2 == 1)
		c = m;
	while (d>1)
	{
		m = m*m;
		d = d / 2;
		if (d % 2 == 1)
			c = c * m;
	};
	return c;
}

BigNat powmod(const BigNat _a, const BigNat _b, const BigNat _mod)
{
	if (_b == 0)
		return !(_a == 0);
	if (_b < 0)
		return 0;
	BigNat c, d, m;
	d = _b;
	c = 1;
	m = _a % _mod;
	if (d % 2 == 1)
		c = m;
	while (d>1)
	{
		m = (m * m) % _mod;
		d = d / 2;
		if (d % 2 == 1)
			c = (c * m) % _mod;
	};
	return c;
}

BigNat NOD(BigNat _a, BigNat _b)
{
	BigNat a, b;

	a = _a;
	b = _b;

	while (a != 0 && b != 0)
	{
		a = a - (a / b)*b;
		if (a != 0)
			b = b - (b / a)*a;
	}
	return b + a;
}


BigPol POLY_NOD(BigPol _a, BigPol _b)
{
	BigPol a, b;

	a = _a;
	b = _b;

	while (a != (std::string)"0" && b != (std::string)"0")
	{
		a = (a / b);
		if (a != (std::string)"0")
			b = (b / a);
	}
	return b + a;
}

void diaf(BigInt _a, BigInt _b, BigInt _c)
{
	if (_a == 0)
	{
		if (_b == 0)
			if (_c == 0)
				puts("Нет решений");
			else
				puts("x,y-любые числа");
		else
		{
			if (_c%_b == 0)
			{
				printf("x=");
				(_c / _b).Put();
			}
			else
				puts("Нет решений");
		}

		return;
	}

	if (_b == 0)
	{
		if (_c%_a == 0)
		{
			printf("y=");
			(_c / _a).Put();
		}
		else
			puts("Нет решений");
		return;
	}

	BigNat a, b,c,d,aa,bb;
	BigInt a0, a1;
	BigInt b0, b1;
	BigInt xk, xa;
	BigInt yk, ya;

	a = _a.a;
	b = _b.a;
	c = _c.a;

	d = NOD(a, b);

	if (c % d != 0)
	{
		puts("Нет решений");
		return;
	}

	a = a / d;
	b = b / d;
	c = c / d;

	a0 = 1;
	a1 = 0;

	b0 = 0;
	b1 = 1;

	while (a != 0 && b != 0)
	{
		aa = (a / b);
		a0 = a0 - aa*b0;
		a1 = a1 - aa*b1;
		a = a - aa*b;
		if (a != 0)
		{
			bb = (b / a);
			b0 = b0 - bb*a0;
			b1 = b1 - bb*a1;
			b = b - (b / a)*a;
		}
	}

	if (a == 1)
	{
		xk = b0;
		yk = b1;
		xa = a0;
		ya = a1;
	}
	if (b == 1)
	{
		xk = a0;
		yk = a1;
		xa = b0;
		ya = b1;
	}

	xa = xa*c*_c.sign;
	ya = ya*c*_c.sign;

	if (_a < 0)
	{
		xa = -xa;
		xk = -xk;
	}

	if (_b < 0)
	{
		ya = -ya;
		yk = -yk;
	}

	if (xk < 0 && yk < 0)
	{
		xk = -xk;
		yk = -yk;
	}

	

	printf("x=");
	xa.Put();
	if (xk>0)
		printf("+");
	xk.Put();
	puts("k");
	printf("y=");
	ya.Put();
	if (yk>0)
		printf("+");
	yk.Put();
	puts("k");


}

BigPol BigPol::deg(BigPol _a)
{
	if (_a.d.size() < 2)
		return (std::string)"0";

	int i;
	BigPol a;

	a = _a;
	
	for (i = 0; i < _a.d.size(); i++)
		a.d[i] = a.d[i] * i;

	for (i = 0; i < _a.d.size()-1; i++)
		a.d[i] = a.d[i+1];

	a.d.resize(_a.d.size() - 1);

	return a;
}

BigNat NOK(BigNat _a, BigNat _b)
{
	return _a / NOD(_a, _b) * _b;
}