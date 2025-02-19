// जय श्री राम !!
// Do not get stuck on a single approach for long !, think of multiple ways !!
// Always use pen and paper to solve ! Don't directly jump into coding !!
#include <bits/stdc++.h>
using namespace std;
#define code by Murari Pandey Infinite1400 from NITKKR
#define yes cout << "YES" << endl
#define no cout << "NO" << endl
#define endl "\n"
#define br break
#define loop(i, n) for (int i = 0; i < n; i++)
#define loop1(i, n) for (int i = 1; i <= n; i++)
#define loopn(i, n) for (int i = n - 1; i >= 0; i--)
#define maap map<int, int>
#define minpq priority_queue<int, vector<int>, greater<int>>
#define vec vector<int>
#define int long long int
#define mod 998244353
#define mod1 1000000007
#define decimal(n) fixed << setprecision(n)
const int N = 1e5 + 5;
bool isPrime[N];
vector<int> primes;
// (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧
bool compare(pair<int, int> &i, pair<int, int> &j) { return i.second < j.second; }
bool comp(pair<int, string> &a, pair<int, string> &b) { return a.second < b.second; }

class DSU
{
private:
	vector<int> rank, parent, size;

public:
	DSU(int n)
	{
		rank.resize(n + 1, 0);
		parent.resize(n + 1);
		size.resize(n + 1, 1);
		for (int i = 0; i < n + 1; i++)
		{
			parent[i] = i;
		}
	}

	int findUparent(int u)
	{
		// Base case if node parent is node itself. so.. it is ultimate parent...
		if (u == parent[u])
		{
			return u;
		}
		// Path compression will be done.. along with finding ultimate parent of node u
		// we will connect node to its ultimate parent...
		return parent[u] = findUparent(parent[u]);
	}

	void union_by_rank(int u, int v)
	{
		int ultp_u = findUparent(u);
		int ultp_v = findUparent(v);
		if (ultp_u == ultp_v)
		{
			return;
		}
		if (rank[ultp_u] < rank[ultp_v])
		{
			parent[ultp_u] = ultp_v;
		}
		else if (rank[ultp_v] < rank[ultp_u])
		{
			parent[ultp_v] = ultp_u;
		}
		else
		{
			// for same rank we can connect either ultp_u--->ultp_v or ultp_v--->ultp_u
			parent[ultp_v] = ultp_u;
			// inc rank
			rank[ultp_u]++;
		}
	}

	void union_by_size(int u, int v)
	{
		int ultp_u = findUparent(u);
		int ultp_v = findUparent(v);
		if (ultp_u == ultp_v)
		{
			return;
		}
		// if size of ultimate parent of u is greater than v then store parent of ultimate parent u
		// to ultimate parent v
		// increase size of ultimate parent of v
		if (size[ultp_u] < size[ultp_v])
		{
			parent[ultp_u] = ultp_v;
			size[ultp_v] += size[ultp_u];
		}
		// else parent of ultimate parent of v to ultimate parent of u and increase size of
		// ultimate parent of u
		else
		{
			parent[ultp_v] = ultp_u;
			size[ultp_u] += size[ultp_v];
		}
	}
};

class SegmentTree
{
private:
	int size;

public:
	typedef struct node
	{
		int sum;
		int max_seg;
		int max_pref;
		int max_suff;
	} node;
	vector<node> values;
	node neutral = {0, 0, 0, 0};
	// Segment Tree can be build for any associative operation a x (b x c) == (a x b) x c where x is associative operation
	SegmentTree(int n)
	{
		// initialise size of segment tree.. to nearest power of 2
		size = 1;
		while (size < n)
			size *= 2;
		values.resize(2 * size);
	}
	// build segment tree of a given size of array in O(n)
	node merge(node left, node right)
	{
		return {
			left.sum + right.sum,
			max(left.max_seg, max(right.max_seg, left.max_suff + right.max_pref)),
			max(left.max_pref, left.sum + right.max_pref),
			max(right.max_suff, right.sum + left.max_suff)};
	}
	node single(int v)
	{
		if (v > 0)
		{
			return {v, v, v, v};
		}
		else
		{
			return {v, 0, 0, 0};
		}
	}
	void build(vector<int> &arr, int x, int lx, int rx)
	{
		// if leaf node just assign the array value
		if (rx - lx == 1)
		{
			// since we have values array of extra size to make to nearst power of 2
			// we have to check that the left segment belongs to array
			if (lx < (int)arr.size())
			{
				values[x] = single(arr[lx]);
			}
			return;
		}
		// else call left child and right child
		int m = (lx + rx) / 2;
		build(arr, 2 * x + 1, lx, m);
		build(arr, 2 * x + 2, m, rx);
		values[x] = merge(values[2 * x + 1], values[2 * x + 2]);
	}
	void build(vector<int> &arr)
	{
		build(arr, 0, 0, size);
	}
	// set function set(i,v) which is to set arr[i]=v
	void set(int i, int v, int x, int lx, int rx)
	{
		// if segment size is 1 meaning leaf node ... just assign the value
		if (rx - lx == 1)
		{
			values[x] = single(v);
			return;
		}
		// if not leaf ! then we have to decide in which segment i belongs to
		int m = (lx + rx) / 2;
		// if i is on left of m then call left segment
		// else call right segmet
		if (i < m)
			set(i, v, 2 * x + 1, lx, m);
		else
			set(i, v, 2 * x + 2, m, rx);
		// after recursion we have to recalculate operation at node x to its left child and right child
		values[x] = merge(values[2 * x + 1], values[2 * x + 2]);
	}

	void set(int i, int v)
	{
		// we will call the above function from root to i
		set(i, v, 0, 0, size);
	}
	node operation(int l, int r, int x, int lx, int rx)
	{
		// case-1 when segment [lx---rx] don't interset with segment[l---r] return 0
		if (lx >= r || l >= rx)
			return neutral;
		// case-2 when segment [lx--rx] completly inside of segment[l---r] return segment operation
		else if (l <= lx && rx <= r)
			return values[x];
		// else go to left child and right child to find segment and return operation
		int m = (lx + rx) / 2;
		node left = operation(l, r, 2 * x + 1, lx, m);
		node right = operation(l, r, 2 * x + 2, m, rx);
		return merge(left, right);
	}
	// operation for a segment from [l----r]
	node operation(int l, int r)
	{
		return operation(l, r, 0, 0, size);
	}

	int first_above(int k, int x, int lx, int rx)
	{
		// element is not founded !!
		if (values[x].sum < k)
			return -1;
		if (rx - lx == 1)
			return lx;
		int m = (lx + rx) / 2;
		// find element in left subtree
		int res = first_above(k, 2 * x + 1, lx, m);
		// if element isn't present in left subtree go to right subtree
		if (res == -1)
		{
			res = first_above(k, 2 * x + 2, m, rx);
		}
		return res;
	}
	int first_above(int k)
	{
		return first_above(k, 0, 0, size);
	}

	int find(int k, int x, int lx, int rx)
	{
		// if leaf node is reached ! meaning kth one is found... return index--lx
		if (rx - lx == 1)
		{
			return lx;
		}
		// find where to locate kth 1
		int m = (lx + rx) / 2;
		// if no. of 1 present in left subtree is strictly smaller means we have to go to left subtree...
		int sl = values[2 * x + 1].sum;
		if (k < sl)
		{
			find(k, 2 * x + 1, lx, m);
		}
		else
		{
			find(k - sl, 2 * x + 2, m, rx);
		}
	}
	int find(int k)
	{
		return find(k, 0, 0, size);
	}
};

void sieve()
{
	fill(isPrime + 2, isPrime + N, true);
	for (int i = 2; i * i < N; i++)
	{
		for (int j = i * i; j < N; j += i)
		{
			isPrime[j] = false;
		}
	}
	for (int i = 2; i < N; i++)
	{
		if (isPrime[i])
		{
			primes.push_back(i);
		}
	}
}

int calcPrime(int n)
{
	// prime number till n
	sieve();
	int res = 0;
	for (int i : primes)
	{
		if (i * i > n)
		{
			break;
		}
		while (n % i == 0)
		{
			n /= i;
			res++;
		}
	}
	if (n > 1)
	{
		res++;
	}
	return res;
}

bool perfectsquare(int n)
{

	if (ceil((double)sqrt(n)) == floor((double)sqrt(n)))
	{
		cout << "perfect square";
		return true;
	}
	else
	{
		cout << "not a perfect square";
		return false;
	}
}

int countSC(int N)
{
	int res = (int)sqrt(N) - (int)cbrt(N);
	return res;
}

bool isprime(int n)
{
	// n is prime or not
	if (n == 1)
	{
		return false;
	}
	if (!(n % 2) && n > 2)
	{
		return false;
	}
	if (n == 2)
	{
		return true;
	}
	else
	{
		for (int i = 3; i * i <= n; i += 2)
		{
			if (n % i == 0)
			{
				return false;
			}
		}
	}
	return true;
}

int smallestDivisor(int n)
{
	if (n % 2 == 0)
		return 2;
	for (int i = 3; i * i <= n; i += 2)
	{
		if (n % i == 0)
			return i;
	}
	return n;
}

int sumarray(int arr[], int n)
{
	// operation of array
	int x = 0;
	for (int i = 0; i < n; i++)
		x += arr[i];
	return x;
}
template <typename T>
T maximum(T a, T b)
{
	if (a > b)
		return a;
	else
		return b;
}
template <typename T>
T minimum(T a, T b)
{
	if (a > b)
		return b;
	else
		return a;
}

int maxarray(int arr[], int n)
{
	// minimum array
	int m = -5e18;
	for (int i = 0; i < n; i++)
	{
		if (m < arr[i])
		{
			m = arr[i];
		}
	}
	return m;
}

int minarray(int arr[], int n)
{
	// minimum array
	int m = 5e18;
	for (int i = 0; i < n; i++)
	{
		if (m > arr[i])
		{
			m = arr[i];
		}
	}
	return m;
}
template <typename T>
void printarray(T arr[], int n)
{
	// array print
	for (int i = 0; i < n; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}
template <typename T>
void printvector(vector<T> &v)
{
	// vector print
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}

bool ispalindrome(string str)
{
	// palindrome or not
	int n = str.length();
	int x = n - 1;
	for (int i = 0; i < n / 2; i++)
	{
		if (str[i] != str[x])
		{
			return false;
		}
		x--;
	}
	return true;
}
bool sorted(vector<int> &v)
{
	// sorted array or not
	for (int i = 0; i < v.size() - 1; i++)
	{
		if (v[i] > v[i + 1])
		{
			return false;
		}
	}
	return true;
}
bool rsorted(vector<int> &v)
{
	// reverse sorted array or not
	for (int i = 0; i < v.size() - 1; i++)
	{
		if (v[i] < v[i + 1])
		{
			return false;
		}
	}
	return true;
}
int smallestprimefactor(int n)
{
	// smallest factor to a number !
	if (n % 2 != 0)
	{
		return 2;
	}
	for (int i = 3; i <= sqrt(n); i += 2)
	{
		if (n % i != 0)
		{
			return i;
		}
	}
	return sqrt(n) + 1;
}

int digitsum(int n)
{
	// Digit operation of a number
	int operation = 0;
	while (n)
	{
		operation += n % 10;
		n /= 10;
	}
	return operation;
}
int minvector(vector<int> &v)
{
	// minimum of vector
	int m = 5e18;
	for (int i = 0; i < v.size(); i++)
	{
		m = minimum(m, v[i]);
	}
	return m;
}
bool ispowerTwo(int n)
{
	return (n & (n - 1)) == 0;
}
int maxvector(vector<int> &v)
{
	// maximum of vector
	int m = -5e18;
	for (int i = 0; i < v.size(); i++)
	{
		m = maximum(m, v[i]);
	}
	return m;
}
vector<int> arra(int a)
{
	// number to vector digit by digit
	vector<int> x;
	while (a)
	{
		x.push_back(a % 10);
		a /= 10;
	}
	reverse(x.begin(), x.end());
	return x;
}
bool ispermutation(int arr[], int n)
{
	// check if permutation or not
	sort(arr, arr + n);
	for (int i = 0; i < n; i++)
	{
		// st.insert(arr[i]);
		if (arr[i] != i + 1)
		{
			return false;
		}
	}
	return true;
}
vector<int> convertbits(int n)
{
	// number to binary vector
	vector<int> v;
	int i = 30;
	while (i--)
	{
		if (n % 2 == 0)
		{
			v.push_back(0);
		}
		else
		{
			v.push_back(1);
		}
		n /= 2;
	}
	reverse(v.begin(), v.end());
	return v;
}
int power(int x, int y)
{
	// power Function
	int temp;
	if (y == 0)
		return 1;
	temp = power(x, y / 2);
	if (y % 2 == 0)
		return (temp * temp) % mod1;
	else
		return (x * temp * temp) % mod1;
}
int ceil(int a, int b)
{
	int x = a / b;
	if (a % b != 0)
		x++;
	return x;
}
int convert_to_number(vector<int> &v)
{
	int ans = 0;
	int a = 1;
	reverse(v.begin(), v.end());
	for (int i = 0; i < v.size(); i++)
	{
		int a = power(2, i);
		if (v[i] == 1)
		{
			ans += a;
		}
	}
	return ans;
}
vector<int> suffix(int arr[], int n)
{
	vector<int> suff(n + 1, 0);
	int x = 1;
	for (int i = n - 1; i >= 0; i--)
	{
		suff[x] = suff[x - 1] + arr[i];
		x++;
	}
	return suff;
}

vector<int> prefix(int arr[], int n)
{
	vector<int> pref(n + 1, 0);
	for (int i = 0; i < n; i++)
	{
		pref[i + 1] = pref[i] + arr[i];
	}
	return pref;
}

int lower_bound(int arr[], int n, int x)
{
	int low = 0;
	int high = n - 1;
	int ans = n;
	while (high >= low)
	{
		int mid = (high + low) / 2;
		if (arr[mid] >= x)
		{
			ans = mid;
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	return ans;
}

int upper_bound(int arr[], int n, int x)
{
	int low = 0, high = n - 1;
	int ans = n;
	while (high > low)
	{
		int mid = (low + high) / 2;
		if (arr[mid] > x)
		{
			ans = mid;
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	return ans;
}
void setbits()
{
	int bits[20][200001] = {0};
	// storing numbers from 0 to 200000 set bits
	// like for 0th position is storing the count of number from 0 to 200000 that has first bit set
	// likewise till 20 twenty..
	// lamen
	// storing count of number that has 1 to 20 bits are set..
	for (int i = 0; i < 20; i++)
	{
		for (int j = 1; j < 200001; j++)
		{
			bits[i][j] = bits[i][j - 1] + (j >> i & 1);
		}
	}
}
static bool com(pair<int, pair<int, int>> &a, pair<int, pair<int, int>> &b)
{
	if (a.first == b.first)
	{
		if (a.second.first == b.second.first)
		{
			return a.second.second < b.second.second;
		}
		else
		{
			return a.second.first < b.second.first;
		}
	}
	return a.first > b.first;
}
bool ispalindrome(int n)
{
	int a = n;
	int res = 0;
	while (n)
	{
		int r = n % 10;
		res = res * 10 + r;
		n /= 10;
	}
	if (a == res)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void infinite()
{
    int n;cin>>n;
	vector<int> arr(n);
    for(auto &it : arr) cin>>it;
    printvector(arr);
}
int32_t main()
{
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	// prime Number in a range from 2 to N
	// sieve();
	// setbits();
	// ofstream Myfile("bits.txt");
	// for (int i = 0; i <= 100000+20; i++)
	// {
	// 	if (isprime(i))
	// 	{
	// 		Myfile << i << ", ";
	// 	}
	// }
	// cout<<"done"<<endl;
	int t = 1;
	cin >> t;
	for (int i = 0; i < t; i++)
	{
		infinite();
		// cout<<t;
	}
	cerr << "Time : " << 1000 * ((double)clock()) / (double)CLOCKS_PER_SEC << "ms\n";
	cerr << __DATE__ << endl
		 << __TIME__ << endl;
	return 0;
}