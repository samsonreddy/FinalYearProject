// gmp01.cpp : Defines the entry point for the console application.
//

#include <utility>
#include <iostream>
#include <gmp_util.h>

using namespace std;
struct ECC
{
	mpz_class x, y; // generator point
	mpz_class n; // order
	mpz_class a, b; //constants
	mpz_class p; // y2=(x3+ax+b) mod p
}root;
void doubling(pair<mpz_class, mpz_class>&R, pair<mpz_class, mpz_class>&P)
{
	//	cout<<"DOUBLING "<<P.second<<"\n";
	mpz_class slope;
	slope = (((3 * P.first*P.first) + (root.a))) / (2 * P.second);
	R.first = (root.p + (slope*slope) - (2 * P.first)) % root.p;
	R.second = (root.p + ((slope)*(P.first - R.first)) - (P.second)) % root.p;
}
void add(pair<mpz_class, mpz_class>&R, pair<mpz_class, mpz_class>&P, pair<mpz_class, mpz_class>&Q)
{
	//	cout<<"add "<<Q.first<<" "<<P.first<<endl;
	mpz_class slope;
	slope = ((Q.second - P.second)) / (Q.first - P.first);
	R.first = (root.p + (slope*slope) - (P.first) - (Q.first)) % root.p;
	R.second = (root.p + (slope*(P.first - R.first)) - (P.second)) % root.p;
	cout << "finished\n";
}
void scalarMultiply(pair<mpz_class, mpz_class> &res, struct ECC root, mpz_class k)
{
	bool flag = false;
	pair<mpz_class, mpz_class> temp;
	pair<mpz_class, mpz_class> acc;
	acc.first = root.x;
	acc.second = root.y;
	k = k % root.p;

	while (k>0)
	{
		//mpz_t t;
		//mpz_mod_ui(t, k.get_mpz_t(), 2);
		mpz_class t = k % 2;
		if (t>0)
		{
			if (flag == false)
			{
				temp.first = (root.p + acc.first) % root.p;
				temp.second = (root.p + acc.second) % root.p;
				flag = true;
			}
			else
			{
				add(temp, res, acc);
			}
			res = temp;
		}
		doubling(temp, acc);
		acc = temp;
		k = k / 2;
		/*cout<<acc.first<<" "<<acc.second<<endl;
		cout<<temp.first<<" "<<temp.second<<endl;
		cout<<res.first<<" "<<res.second<<endl;*/
	}
}

mpz_class uncompress_generator_point(mpz_class X, mpz_class A, mpz_class B, mpz_class P)
{
	mpz_t y, C, temp, minm, t1;
	mpz_inits(y, C, temp, minm, t1, NULL);
	mpz_powm_ui(C, X.get_mpz_t(), 3, P.get_mpz_t());		// C = X^3
	mpz_mul(temp, A.get_mpz_t(), X.get_mpz_t());		// temp = AX
	mpz_add(temp, temp, B.get_mpz_t());		// temp = AX + B
	mpz_add(C, C, temp);		// C = C + temp ; C = X^3 + AX + B
	mpz_sqrt(y, C);
	mpz_fdiv_r(y, y, P.get_mpz_t());
	mpz_sub(minm, P.get_mpz_t(), y);
	mpz_class val;
	mpz_sub(t1, y, minm);
	if (t1 > 0)
	{
		return mpz_class(minm);
	}
	return mpz_class(y);
}
void print_class(struct ECC t)
{
	cout << "P : " << t.p << endl;
	cout << "A : " << t.a << endl;
	cout << "B : " << t.b << endl;
	cout << "X : " << t.x << endl;
	cout << "Y : " << t.y << endl;
}
int main()
{
	mpz_class X, Y, N, P, A, B;

		cout << "Input P: ";
	cin >> P;
	cout << "Input A: ";
	cin >> A;
	cout << "Input B: ";
	cin >> B;
	
	P = mpz_class("DB7C 2ABF62E3 5E668076 BEAD208B", 16);
	A = mpz_class("DB7C 2ABF62E3 5E668076 BEAD2088", 16);
	B = mpz_class("659E F8BA0439 16EEDE89 11702B22", 16);
	X = mpz_class("020948 7239995A 5EE76B55 F9C2F098", 16);
	Y = uncompress_generator_point(X, A, B, P);

	root.x = X;
	root.y = Y;
	root.p = P;
	root.a = A;
	root.b = B;

	print_class(root);
	/*int cont;
	do {

		/*		cout << "Input x: ";
		cin >> X;
		cout << "Input y: ";
		cin >> Y;
		
		cout << "Input N: ";
		cin >> N;
		root.n = N;
		pair<mpz_class, mpz_class> res;
		scalarMultiply(res, root, root.n);
		cout << root.n << " * ( " << root.x << " , " << root.y << " ) = " << " ( " << res.first << " , " << res.second << " )" << endl;

		cout << endl << "Press 1 to continue :";
		cin >> cont;

	} while (cont == 1);*/
	cin>>P;

}
