#ifndef INPUTMAPPING_H
#define INPUTMAPPING_H

namespace core
{
class Key
{
public:
    Key(const core::String& name) : m_name(name)
    {
    }

    const core::String& Name() const
    {
        return m_name;
    }

    operator const core::String&() const
    {
        return m_name;
    }

private:
    // Later we want to have hashed name identifier here.
    core::String m_name;
};

inline bool operator==(const Key& key, const core::String& str)
{
    return key.Name() == str;
}

struct Keys {
    static const Key A;
    static const Key B;
    static const Key C;
    static const Key D;
    static const Key E;
    static const Key F;
    static const Key G;
    static const Key H;
    static const Key I;
    static const Key J;
    static const Key K;
    static const Key L;
    static const Key M;
    static const Key N;
    static const Key O;
    static const Key P;
    static const Key Q;
    static const Key R;
    static const Key S;
    static const Key T;
    static const Key U;
    static const Key V;
    static const Key W;
    static const Key X;
    static const Key Y;
    static const Key Z;
};
}

#endif