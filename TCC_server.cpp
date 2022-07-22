// TCC_open.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "tcp.h"
#include "mpc.h"

int main()
{
    std::cout << "Hello server!\n";
    mpc_server user;
    user.init();


    int len = 10;
    ArrayXi x(len);
    for (int i = 0; i < len; i++) {
        x[i] = rand();
    }
    print_array(x);



    ArrayXi shared_x = user.share_v(x);
    ArrayXi top_k = user.STKC(shared_x, 4);
    // print_array(shared_x);
    ArrayXi r_x = user.restore_v(top_k);
    print_array(r_x);

    /**************** SQ protocol test *****************/


    user.tcp.close();
    system("pause");
}

