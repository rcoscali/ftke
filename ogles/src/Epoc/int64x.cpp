// ====================================================================================
//
// Symbian TInt64 extension
//
// ------------------------------------------------------------------------------------
//
// 11-05-2004   Iwan Junianto       initial version
//
// ====================================================================================

#include <int64x.h>

EXPORT_C TInt64X::TInt64X(): TInt64(0,0)
{
}

EXPORT_C TInt64X::TInt64X(TInt aVal): TInt64(aVal)
{
}

EXPORT_C TInt64X::TInt64X(const TInt64& aVal): TInt64(aVal)
{
}

EXPORT_C TInt64X::operator TInt() const
{
	return GetTInt();
}

EXPORT_C TInt64X TInt64X::operator*(const TInt64X& aVal) const
{
    return TInt64X(TInt64::operator*(static_cast<const TInt64&>(aVal)));
}

EXPORT_C TInt64X TInt64X::operator*(TInt aVal) const
{
    return TInt64X(TInt64::operator*(aVal));
}

EXPORT_C TInt64X TInt64X::operator/(const TInt64X& aVal) const
{
    return TInt64X(TInt64::operator/(aVal));
}

EXPORT_C TInt64X TInt64X::operator/(TInt aVal) const
{
    return TInt64X(TInt64::operator/(aVal));
}

EXPORT_C TInt64X TInt64X::operator+(const TInt64X& aVal) const
{
    return TInt64X(TInt64::operator+(aVal));
}

EXPORT_C TInt64X TInt64X::operator-(const TInt64X& aVal) const
{
    return TInt64X(TInt64::operator-(static_cast<const TInt64&>(aVal)));
}

EXPORT_C TInt64X TInt64X::operator>>(TInt aShift) const
{
    return TInt64X(TInt64::operator>>(aShift));
}

EXPORT_C TInt64X TInt64X::operator<<(TInt aShift) const
{
    return TInt64X(TInt64::operator<<(aShift));
}

EXPORT_C TInt TInt64X::operator<(TInt aVal) const
{
    return TInt64::operator<(TInt64(aVal));
}

EXPORT_C TInt TInt64X::operator>(TInt aVal) const
{
    return TInt64::operator>(TInt64(aVal));
}

