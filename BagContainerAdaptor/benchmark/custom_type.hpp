#ifndef CUSTOM_TYPE_HPP
#define CUSTOM_TYPE_HPP

// Custom type used for benchmarking.
class CustomType
{
public:
    int m_x;
    int m_y;

    /// Constructor.
    CustomType(int x = 0, int y = 0)
        : m_x(x), m_y(y)
    {
    }

    /// Operator overload: ==
    bool operator==(const CustomType& other) const
    {
        return (m_x == other.m_x) && (m_y == other.m_y);
    }

    /// Operator overload: <
    bool operator<(const CustomType& other) const
    {
        return (m_x < other.m_x) || ((m_x == other.m_x) && (m_y < other.m_y));
    }
};

#endif
