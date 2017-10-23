#include <cstdlib>
#include <cstring>
#include <string>

const int PREC = 20010;

class Decimal {
    public:
        Decimal();
        Decimal(int x);
        
        bool is_zero() const;
        
        std::string to_string(int p) const;
        
        friend Decimal operator + (const Decimal &a, const Decimal &b);
        
        friend Decimal operator - (const Decimal &a, const Decimal &b);
        
        friend Decimal operator / (const Decimal &a, int x);

        friend bool operator >= (const Decimal &a, const Decimal &b);
        
        Decimal & operator += (const Decimal &b);

        Decimal & operator -= (const Decimal &b);
        
        Decimal & operator *= (int x);
        
        Decimal & operator /= (int x);
        
        friend Decimal operator - (const Decimal &a);
        
    private:
        static const int len = PREC / 9 + 1;
        static const int mo = 1000000000;
        
        static void append_to_string(std::string &s, long long x);
        
        bool is_neg;
        long long integer;
        int data[len];
        
        void init_zero();
        void init(const char *s);
};

Decimal::Decimal() {
    this->init_zero();
}

Decimal::Decimal(int x) {
    this->init_zero();
    
    if (x < 0) {
        is_neg = true;
        x = -x;
    }
    
    integer = x;
}

void Decimal::init_zero() {
    is_neg = false;
    integer = 0;
    memset(data, 0, len * sizeof(int));
}

bool Decimal::is_zero() const {
    if (integer) return false;
    for (int i = 0; i < len; i++) {
        if (data[i]) return false;
    }
    return true;
}

void Decimal::append_to_string(std::string &s, long long x) {
    if (x == 0) {
        s.append(1, 48);
        return;
    }
    
    char _[30];
    int cnt = 0;
    while (x) {
        _[cnt++] = x % 10;
        x /= 10;
    }
    while (cnt--) {
        s.append(1, _[cnt] + 48);
    }
}

std::string Decimal::to_string(int p) const {
    std::string ret;
    
    if (is_neg && !this->is_zero()) {
        ret = "-";
    }
    
    append_to_string(ret, this->integer);
    
    ret.append(1, '.');
    
    for (int i = 0; i < len; i++) {
        // append data[i] as "%09d"
        int x = mo / 10;
        int tmp = data[i];
        while (x) {
            ret.append(1, 48 + tmp / x);
            tmp %= x;
            x /= 10;
            if (--p == 0) {
                break;
            }
        }
        if (p == 0) break;
    }
    
    if (p > 0) {
        ret.append(p, '0');
    }
    
    return ret;
}

bool operator >= (const Decimal &a, const Decimal &b) {
    if (a.is_neg != b.is_neg) {
        return !a.is_neg || (a.is_zero() && b.is_zero());
    } else if (!a.is_neg) {
        // a, b >= 0
        if (a.integer != b.integer) {
            return a.integer > b.integer;
        }
        for (int i = 0; i < Decimal::len; i++) {
            if (a.data[i] != b.data[i]) {
                return a.data[i] > b.data[i];
            }
        }
        return true;
    } else {
        // a, b <= 0
        if (a.integer != b.integer) {
            return a.integer < b.integer;
        }
        for (int i = 0; i < Decimal::len; i++) {
            if (a.data[i] != b.data[i]) {
                return a.data[i] < b.data[i];
            }
        }
        return true;
    }
}

Decimal & Decimal::operator /= (int x) {
    if (x < 0) {
        is_neg ^= 1;
        x = -x;
    }
    
    int last = integer % x;
    integer /= x;
    
    for (int i = 0; i < len; i++) {
        long long tmp = 1LL * last * mo + data[i];
        data[i] = tmp / x;
        last = tmp - 1LL * data[i] * x;
    }
    
    if (is_neg && integer == 0) {
        int i;
        for (i = 0; i < len; i++) {
            if (data[i] != 0) {
                break;
            }
        }
        if (i == len) {
            is_neg = false;
        }
    }
    
    return *this;
}

Decimal & Decimal::operator *= (int x) {
    if (x < 0) {
        is_neg ^= 1;
        x = -x;
    } else if (x == 0) {
        init_zero();
        return *this;
    }
    
    int last = 0;
    for (int i = len - 1; i >= 0; i--) {
        long long tmp = 1LL * data[i] * x + last;
        last = tmp / mo;
        data[i] = tmp - 1LL * last * mo;
    }
    integer = integer * x + last;
    
    return *this;
}

Decimal operator - (const Decimal &a) {
    Decimal ret = a;
    // -0 = 0
    if (!ret.is_neg && ret.integer == 0) {
        int i;
        for (i = 0; i < Decimal::len; i++) {
            if (ret.data[i] != 0) break;
        }
        if (i < Decimal::len) {
            ret.is_neg = true;
        }
    } else {
        ret.is_neg ^= 1;
    }
    return ret;
}

Decimal operator * (const Decimal &a, int x) {
    Decimal ret = a;
    return ret *= x;
}

Decimal operator / (const Decimal &a, int x) {
    Decimal ret = a;
    return ret /= x;
}

Decimal operator + (const Decimal &a, const Decimal &b) {
    if (a.is_neg == b.is_neg) {
        Decimal ret = a;
        bool last = false;
        for (int i = Decimal::len - 1; i >= 0; i--) {
            ret.data[i] += b.data[i] + last;
            if (ret.data[i] >= Decimal::mo) {
                ret.data[i] -= Decimal::mo;
                last = true;
            } else {
                last = false;
            }
        }
        ret.integer += b.integer + last;
        return ret;
    } else if (!a.is_neg) {
        // a - |b|
        return a - -b;
    } else {
        // b - |a|
        return b - -a;
    }
}

Decimal operator - (const Decimal &a, const Decimal &b) {
    if (!a.is_neg && !b.is_neg) {
        if (a >= b) {
            Decimal ret = a;
            bool last = false;
            for (int i = Decimal::len - 1; i >= 0; i--) {
                ret.data[i] -= b.data[i] + last;
                if (ret.data[i] < 0) {
                    ret.data[i] += Decimal::mo;
                    last = true;
                } else {
                    last = false;
                }
            }
            ret.integer -= b.integer + last;
            return ret;
        } else {
            Decimal ret = b;
            bool last = false;
            for (int i = Decimal::len - 1; i >= 0; i--) {
                ret.data[i] -= a.data[i] + last;
                if (ret.data[i] < 0) {
                    ret.data[i] += Decimal::mo;
                    last = true;
                } else {
                    last = false;
                }
            }
            ret.integer -= a.integer + last;
            ret.is_neg = true;
            return ret;
        }
    } else if (a.is_neg && b.is_neg) {
        // a - b = (-b) - (-a)
        return -b - -a;
    } else if (a.is_neg) {
        // -|a| - b
        return -(-a + b);
    } else {
        // a - -|b|
        return a + -b;
    }
}

Decimal operator + (const Decimal &a, double x) {
    return a + Decimal(x);
}

Decimal operator + (double x, const Decimal &a) {
    return Decimal(x) + a;
}

Decimal operator - (const Decimal &a, double x) {
    return a - Decimal(x);
}

Decimal operator - (double x, const Decimal &a) {
    return Decimal(x) - a;
}

Decimal & Decimal::operator += (const Decimal &b) {
    *this = *this + b;
    return *this;
}

Decimal & Decimal::operator -= (const Decimal &b) {
    *this = *this - b;
    return *this;
}

#include <cstdio>

int main() {
    Decimal ans = 0;
    Decimal tmp = 20;
    for (int i = 1; !tmp.is_zero(); i++) {
        tmp /= 7;
        if (i%4 == 1)
            ans += tmp / i;
        if (i%4 == 3)
            ans -= tmp / i;
    }
    tmp = 8;
    for (int i = 1; !tmp.is_zero(); i++) {
        tmp *= 3; tmp /= 79;
        if (i%4 == 1)
            ans += tmp / i;
        if (i%4 == 3)
            ans -= tmp / i;
    }
    printf("%s\n",ans.to_string(20000).c_str());
}
