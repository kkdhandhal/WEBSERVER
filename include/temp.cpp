#include <iostream>
#include "responce.cpp"

using namespace std;

int main()
{
    string tmpstr ="1111k2kk111 \n djfdslf dsfkj sdfj  \n eireoiroeior oeiroe\n kkkf \n";
    HTTP_Responce res;

    res.Get_HTTP_file(tmpstr);
}