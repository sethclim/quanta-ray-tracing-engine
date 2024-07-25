
template <typename T>
class Vector3
{
public:
    T x, y, z;

    Vector3(){ }

    Vector3(T _x, T _y, T _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3<T> operator+(Vector3<T> const &obj)
    {
        Vector3 res = Vector3();
        res.x = x + obj.x;
        res.y = y + obj.y;
        res.z = z + obj.z;
        return res;
    }

    Vector3 operator-(Vector3 const &obj)
    {
        Vector3 res = Vector3();
        res.x = x - obj.x;
        res.y = y - obj.y;
        res.z = z - obj.z;
        return res;
    }

    Vector3 operator*(Vector3 const &obj)
    {
        Vector3 res = Vector3();
        res.x = x * obj.x;
        res.y = y * obj.y;
        res.z = z * obj.z;
        return res;
    }
};