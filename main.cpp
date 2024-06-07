#include <bits/stdc++.h>
using namespace std;
#define int long long
int prime[10] = {11,13,29,89,127,881,1009,8861,104729,99961};
pair<int,int>publicKey,privateKey;
vector<int>mi;
int ksm(int a,int b,int p){
    int t = 1;
    while(b){
        if(b&1){
            t = t * a % p;
        }
        a = a*a%p;
        b /= 2;
    }
    return t;
}

int extendedGCD(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int gcd = extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

// 求a关于m的模逆元，若存在则返回模逆元，否则返回-1
int inv(int a, int m) {
    int x, y;
    int gcd = extendedGCD(a, m, x, y);
    if (gcd != 1) {
        // a和m不互质，因此模逆元不存在
        return -1;
    } else {
        // 确保结果为正
        return (x % m + m) % m;
    }
}

void generateKey(){
    srand(time(nullptr));
    int p = prime[rand() % 10];
    int q = 0;
    while((q = prime[rand() % 10]) == p);
    int n = p * q;
    int z = (p-1) * (q-1);
    auto find = [&](int x){
        x++;
        while(1){
            bool ok = 1;
            for(int i=2;i<=sqrt(x);++i){
                if(x%i==0){
                    ok = 0;
                    break;
                }
            }
            if(ok) return x;
            x++;
        }
    };
    int d = find(max(p,q));
    assert(__gcd(d,z)==1);
    int e = inv(d,z); 
    assert(e * d % z == 1);
    publicKey = make_pair(e,n);
    privateKey = make_pair(d,n);
}

string run_encode(string &P){
    string ans;
    mi.clear();
    int e = publicKey.first;
    int n = publicKey.second;
    for(auto x:P){
        int k = ksm(x,e,n);
        mi.push_back(k);
        ans.push_back(k % 128);
    }
    return ans;
}

string run_decode(){
    string ans;
    int d = privateKey.first;
    int n = privateKey.second;
    for(auto x:mi){
        ans.push_back((ksm(x,d,n)));
    }
    return ans;
}

int run_hack(int e,int n){
    cout << "\n-------------------------------------\n";
    auto start = std::chrono::high_resolution_clock::now();
    int p,q;
    for(int i=2;i<=sqrt(n);++i){
        if(n % i==0){
            p = i,q = n/i;
            break;
        }
    }
    int z = (p-1) * (q-1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << "破解用时: " << duration << "\n";
    return inv(e,z);
}

signed main(){
    string P;
    cin >> P;
    string C;
    generateKey(); 
    cout << "公钥: " << "(" << publicKey.first << ", " << publicKey.second << ")\n";
    cout << "私钥: " << "(" << privateKey.first << ", " << privateKey.second << ")\n";
    C = run_encode(P); 
    cout << "密文: \n" << C << "\n";
    cout << "明文: \n" << run_decode(); 
    cout << run_hack(publicKey.first, publicKey.second) << "\n";
    return 0;
}