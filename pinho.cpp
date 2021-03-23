#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <bits/stdc++.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <sys/time.h>

using namespace std;
double gettime( void )
{
    struct timeval ttime;
    gettimeofday( &ttime , 0 );
    return ttime.tv_sec + ttime.tv_usec * 0.000001;
};
typedef unsigned char uchar;
#define pred(i, h) ((t=(i)-(h))<0?  t+n: t)
#define succ(i, h) ((t=(i)+(h))>=n? t-n: t)

typedef struct Element Element;
struct Element
{
    int a,b,d;
    Element *suivant;
};

typedef struct Pair Pair;
struct Pair
{
    char a,b;
};

typedef struct Element2 Element2;
struct Element2
{
    int a,b,d;

};

typedef struct Pile Pile;
struct Pile
{
    Element *premier;
};


struct AhoCorasick
{
    map<int, map <char, int> > go_to;
    vector<int> failure;
    map<int, vector <int> > output;
    vector<list<int> > positions;

    void create_goto(const vector<string> &patterns) {

        positions.resize(patterns.size());

        int new_state = 0;
        for (int i = 0; i < (int)patterns.size(); i++)
        {
            int state = 0, j;
            for (j = 0; j <(int) patterns[i].size() && go_to[state].count(patterns[i][j]) >= 1; j++)
                state = go_to[state][patterns[i][j]];

            for (int k = j; k < (int)patterns[i].size(); k++)
            {
                go_to[state][patterns[i][k]] = ++new_state;
                state = new_state;
            }

            if (output.find(state) == output.end())
                output[state] = vector <int> (1, i);
            else
                output[state].push_back(i);
        }

        failure.resize(new_state + 1, 0);
        for (char i = ' '; i <= '~'; i++)
            if (go_to[0].count(i) == 0)
                go_to[0][i] = 0;

        create_failure();
    }

    void create_failure() {

        queue<int> q;
        int current, state;
        for (map<char, int>::iterator itr = go_to[0].begin(); itr != go_to[0].end(); itr++)
        {
            if ((*itr).second != 0)
            {
                q.push ((*itr).second);
                failure[(*itr).second] = 0;
            }
        }

        while (!q.empty())
        {
            current = q.front();
            q.pop();
            state = failure[current];

            for (map<char, int>::iterator itr = go_to[current].begin(); itr != go_to[current].end(); itr++)
            {
                if (go_to[current].count((*itr).first) != 0)
                {
                    q.push((*itr).second);
                    state = failure[current];
                    while (go_to[state].count((*itr).first) == 0)
                        state = failure[state];
                    failure[(*itr).second] = go_to [state][(*itr).first];
                    output[(*itr).second].insert(output[(*itr).second].end(), output[failure[(*itr).second]].begin(),
                        output[failure[(*itr).second]].end());
                }
            }
        }
    }

    void find_matches(const string &T)
    {
        int state = 0, t;
        for (int i = 0; i <(int) T.size(); i++)
        {
            while (go_to[state].count(T[i]) == 0)
                state = failure[state];

            state = go_to [state][T[i]];
            if (output.count(state) > 0)
            {
                t = output[state].size();
                vector <int> tmp = output[state];
                for (int j = 0; j < t; j++)
                    positions[tmp[j]].push_back(i);
            }
        }
    }
};


enum
{
    BUCK = ~(~0u>>1), /* high bit */
    MAXI = ~0u>>1,    /* biggest int */
};

static  void  qsort2(int*, int*, int n);
static  int ssortit(int a[], int p[], int n, int h, int *pe, int nbuck);

int
sarray(int a[], int n)
{
    int i, l;
    int c, cc, ncc, lab, cum, nbuck;
    int k;
    int *p = 0;
    int result = -1;
    int *al;
    int *pl;

    for(k=0,i=0; i<n; i++)
        if(a[i] > k)
            k = a[i]; /* max element */
    k++;
    if(k>n)
        goto out;

    nbuck = 0;
    p = new int[n];
    if(p == 0)
        goto out;


    pl = p + n - k;
    al = a;
    memset(pl, -1, k*sizeof(int));

    for(i=0; i<n; i++) {    /* (1) link */
        l = a[i];
        al[i] = pl[l];
        pl[l] = i;
    }

    for(i=0; i<k; i++)    /* check input - no holes */
        if(pl[i]<0)
            goto out;


    lab = 0;      /* (2) create p and label a */
    cum = 0;
    i = 0;
    for(c = 0; c < k; c++){
        for(cc = pl[c]; cc != -1; cc = ncc){
            ncc = al[cc];
            al[cc] = lab;
            cum++;
            p[i++] = cc;
        }
        if(lab + 1 == cum) {
            i--;
        } else {
            p[i-1] |= BUCK;
            nbuck++;
        }
        lab = cum;
    }

    result = ssortit(a, p, n, 1, p+i, nbuck);
    memcpy(a, p, n*sizeof(int));

out:
    free(p);
    return result;
}

/* bsarray(uchar buf[], int p[], int n)
 * The input, buf, is an arbitrary byte array of length n.
 * The input is copied to temporary storage, relabeling
 * pairs of input characters and appending a unique end marker
 * having a value that is effectively less than any input byte.
 * The suffix array of this extended input is computed and
 * stored in p, which must have length at least n+1.
 *
 * Returns the index of the identity permutation (regarding
 * the suffix array as a list of circular shifts),
 * or -1 if there was an error.
 */
int
bsarray(const uchar buf[], int p[], int n)
{
    int *a, buckets[256*256];
    int i, last, cum, c, cc, ncc, lab, id, nbuck;

    a = new int[n+1];
    if(a == 0)
        return -1;


    memset(buckets, -1, sizeof(buckets));
    c = buf[n-1] << 8;
    last = c;
    for(i = n - 2; i >= 0; i--){
        c = (buf[i] << 8) | (c >> 8);
        a[i] = buckets[c];
        buckets[c] = i;
    }

    /*
     * end of string comes before anything else
     */
    a[n] = 0;

    lab = 1;
    cum = 1;
    i = 0;
    nbuck = 0;
    for(c = 0; c < 256*256; c++) {
        /*
         * last character is followed by unique end of string
         */
        if(c == last) {
            a[n-1] = lab;
            cum++;
            lab++;
        }

        for(cc = buckets[c]; cc != -1; cc = ncc) {
            ncc = a[cc];
            a[cc] = lab;
            cum++;
            p[i++] = cc;
        }
        if(lab == cum)
            continue;
        if(lab + 1 == cum)
            i--;
        else {
            p[i - 1] |= BUCK;
            nbuck++;
        }
        lab = cum;
    }

    id = ssortit(a, p, n+1, 2, p+i, nbuck);
    free(a);
    return id;
}

static int
ssortit(int a[], int p[], int n, int h, int *pe, int nbuck)
{
    int *s, *ss, *packing, *sorting;
    int v, sv, vv, packed, lab, t, i;

    for(; h < n && p < pe; h=2*h) {
        packing = p;
        nbuck = 0;

        for(sorting = p; sorting < pe; sorting = s){
            /*
             * find length of stuff to sort
             */
            lab = a[*sorting];
            for(s = sorting; ; s++) {
                sv = *s;
                v = a[succ(sv & ~BUCK, h)];
                if(v & BUCK)
                    v = lab;
                a[sv & ~BUCK] = v | BUCK;
                if(sv & BUCK)
                    break;
            }
            *s++ &= ~BUCK;
            nbuck++;

            qsort2(sorting, a, s - sorting);

            v = a[*sorting];
            a[*sorting] = lab;
            packed = 0;
            for(ss = sorting + 1; ss < s; ss++) {
                sv = *ss;
                vv = a[sv];
                if(vv == v) {
                    *packing++ = ss[-1];
                    packed++;
                } else {
                    if(packed) {
                        *packing++ = ss[-1] | BUCK;
                    }
                    lab += packed + 1;
                    packed = 0;
                    v = vv;
                }
                a[sv] = lab;
            }
            if(packed) {
                *packing++ = ss[-1] | BUCK;
            }
        }
        pe = packing;
    }

    /*
     * reconstuct the permutation matrix
     * return index of the entire string
     */
    v = a[0];
    for(i = 0; i < n; i++)
        p[a[i]] = i;

    return v;
}

/*
 * qsort from Bentley and McIlroy, Software--Practice and Experience
   23 (1993) 1249-1265, specialized for sorting permutations based on
   successors
 */
static void
vecswap2(int *a, int *b, int n)
{
    while (n-- > 0) {
            int t = *a;
        *a++ = *b;
        *b++ = t;
    }
}

#define swap2(a, b) { t = *(a); *(a) = *(b); *(b) = t; }

static int*
med3(int *a, int *b, int *c, int *asucc)
{
    int va, vb, vc;

    if ((va=asucc[*a]) == (vb=asucc[*b]))
        return a;
    if ((vc=asucc[*c]) == va || vc == vb)
        return c;
    return va < vb ?
          (vb < vc ? b : (va < vc ? c : a))
        : (vb > vc ? b : (va < vc ? a : c));
}

static void
inssort(int *a, int *asucc, int n)
{
    int *pi, *pj, t;

    for (pi = a + 1; --n > 0; pi++)
        for (pj = pi; pj > a; pj--) {
            if(asucc[pj[-1]] <= asucc[*pj])
                break;
            swap2(pj, pj-1);
        }
}

static void
qsort2(int *a, int *asucc, int n)
{
    int d, r, partval;
    int *pa, *pb, *pc, *pd, *pl, *pm, *pn, t;

    if (n < 15) {
        inssort(a, asucc, n);
        return;
    }
    pl = a;
    pm = a + (n >> 1);
    pn = a + (n-1);
    if (n > 30) { /* On big arrays, pseudomedian of 9 */
        d = (n >> 3);
        pl = med3(pl, pl+d, pl+2*d, asucc);
        pm = med3(pm-d, pm, pm+d, asucc);
        pn = med3(pn-2*d, pn-d, pn, asucc);
    }
    pm = med3(pl, pm, pn, asucc);
    swap2(a, pm);
    partval = asucc[*a];
    pa = pb = a + 1;
    pc = pd = a + n-1;
    for (;;) {
        while (pb <= pc && (r = asucc[*pb]-partval) <= 0) {
            if (r == 0) {
                swap2(pa, pb);
                pa++;
            }
            pb++;
        }
        while (pb <= pc && (r = asucc[*pc]-partval) >= 0) {
            if (r == 0) {
                swap2(pc, pd);
                pd--;
            }
            pc--;
        }
        if (pb > pc)
            break;
        swap2(pb, pc);
        pb++;
        pc--;
    }
    pn = a + n;
    r = pa-a;
    if(pb-pa < r)
        r = pb-pa;
    vecswap2(a, pb-r, r);
    r = pn-pd-1;
    if(pd-pc < r)
        r = pd-pc;
    vecswap2(pb, pn-r, r);
    if ((r = pb-pa) > 1)
        qsort2(a, asucc, r);
    if ((r = pd-pc) > 1)
        qsort2(a + n-r, asucc, r);
}

int
lcpa(const int *a, const char *s0, int *lcp, int n)
{
    int i, h;
    uchar *s = (uchar*)s0;
    int *inv = (int*)malloc(n*sizeof(int));

    if(inv == 0)
        return 0;
    for(i=0; i<n; i++)
        inv[a[i]] = i;

    h = 0;      /* visit in string order */
    for(i=0; i<n-1; i++) {  /* omit last, least suff */
        int x = inv[i]; /* i,j,x,h as in intro */
        int j = a[x-1];
        uchar *p1 = s + i + h;
        uchar *p0 = s + j + h;
        while(*p1++ == *p0++)
            h++;
        lcp[x] = h;
        if(h > 0)
            h--;
    }
    lcp[0] = 0; /* least suffix has no predecessor */
    free(inv);
    return 1;
}


int*
lcp(const int *a, const char *s, int n)
{
    int *lcp = (int*)malloc(n*sizeof(int));

    if(lcp == 0)
        return 0;
    if(lcpa(a, s, lcp, n) == 0) {
        free(lcp);
        return 0;
    }
    return lcp;
}

/* lcpa is used by the java native method */



uchar *codetab(const uchar *s)
{
    int i, n;
    uchar *t = (uchar*)calloc(256,1);
    if(t) {
        for( ; *s; s++)
            t[*s] = 1;
        for(i=n=1; i<256; i++)
            if(t[i])
                t[i] = n++;
    }
    return t;
}



int *scode(const char *s0)
{
    int i;
    const uchar *s = (const uchar*)s0;
    uchar *t = codetab((uchar*)s);
    int *r = (int*)malloc((strlen((char*)s)+1)*sizeof(int));
    if(t && r)
        for(i=0; ; i++) {
            r[i] = t[s[i]];
            if(s[i] == 0)
                break;
        }
    free(t);
    return r;
}


uchar *inverse(const uchar *t)
{
    int i;
    uchar *r = (uchar*)calloc(256,1);
    if(r==0 || t==0)
        return 0;
    for(i=0; i<256; i++)
        if(t[i])
            r[t[i]] = i;
    return r;
}

vector<string> ac(vector<string> v,string   a)
{

    int i;
    AhoCorasick AC;
    int NBRL=0,TF=0;




    string s="";


    ifstream fichier(a);



   if(fichier)
   {

      string ligne="";

      while(getline(fichier, ligne))
      {
         s=s+ligne;
         NBRL++;
      }


   }
   else
   {
      cout << "ERROR: Could not open file for reading." << endl;
   }
    TF=s.size();
    AC.create_goto(v);
    AC.find_matches(string(s));
    vector<string> aa;
    cout<<v.size()<<endl;
    for (i = 0; i < (int)AC.positions.size(); i++)
    {
        int cpp=0;
            for (list<int>::iterator itr = AC.positions[i].begin(); itr != AC.positions[i].end(); itr++){

                cpp++;
            }

            if(cpp!=0){
               aa.push_back(v[i]);
            }

    }



    string const nomFichier("result2.txt");
    ofstream monFlux(nomFichier.c_str());

    if(monFlux)
    {
        monFlux << "The list of minimal absent words that are absent from humans DNA but present in virus DNA" << endl;
    }
    else
    {
        cout << "ERROR: Could not open file" << endl;
    }


    for (int i = 0; i <(int)aa.size(); ++i)
    {
       monFlux<<aa[i]<<endl;

    }
    aa.insert(aa.begin(),std::to_string(((float)TF*0.001)));
    aa.insert(aa.begin(),std::to_string(NBRL));
    return aa;

}

vector<string>   pinho(int taille3, char lefichier[])
{
    char *S,Y[100];
        int taille,taille_maw,tailledey=0,TF=0,NBRL=0;
        float temps;
        clock_t t1, t2;
        vector<string> info;


        ifstream fichier(lefichier);



       if(fichier)
       {


          string ligne,text="";

          while(getline(fichier, ligne))
          {
            if(ligne[0]!='>'){
             text=text+ligne;
             NBRL++;}

          }
          taille=text.length();
          TF=text.length();
          S=new char[taille+7];
          strcpy(S,(char*)text.c_str());


       }
       else
       {
          cout << "ERROR: Could not open file for reading." << endl;
       }






        vector<string>  maw;






        taille_maw=taille3;
        printf("fichier est lu \n");

        t1 = clock();
        //t1 = gettime();
        int *l;
        int n = strlen(S)+1;
        int *a = scode(S);
        sarray(a, n);
        l = lcp(a, S, n);
        l[n]=0;
        printf("lcp et sa gnéré\n");

        strcat(S,"$");
       for(int k=strlen(S);k>=0;k--){
                if(k!=0){
                    S[k]=S[k-1];
                }else{
                    S[k]='#';
                }
        }

        printf("$ et # sont mis\n");

        stack<Element2> P2;

        P2.push({0,0,0});

       Pair aaa;

        for(int k=2;k<n+1;k++)
        {
            int i=k-1;
            while(l[k]<(P2.top().d))
            {
                Element2 lcpi=P2.top();

                P2.pop();
                lcpi.b=k-1;


                if(lcpi.d==taille3-2){

                    std::vector<char> r;
                    r.clear();

                    std::vector<char> le;
                    le.clear();

                    std::vector<char> le2;
                    le2.clear();

                    std::vector<Pair> c;
                    c.clear();

                    int longr=0,longl=0,longl2=0,longc=0;

                    for(int ki=lcpi.a;ki<=lcpi.b;ki++)
                    {


                        le.push_back(S[a[ki]]);
                        if(S[a[ki]]!='#'){

                               std::vector<char>::iterator it = std::find(le2.begin(), le2.end(),S[a[ki]] );
                               if (it == le2.end())
                                  le2.push_back(S[a[ki]]);
                        }
                        if(S[a[ki]+lcpi.d+1]!='$'){

                            std::vector<char>::iterator it = std::find(r.begin(), r.end(),S[a[ki]+lcpi.d+1] );
                            if (it == r.end())
                                  r.push_back(S[a[ki]+lcpi.d+1]);


                        }


                        if((S[a[ki]]!='#')&&(S[a[ki]+1+lcpi.d]!='$'))
                        {



                                c.push_back({S[a[ki]],S[a[ki]+lcpi.d+1]});




                        }

                    }


                    if(le.size()>1)
                    {

                        for(int jl=0;jl<le2.size();jl++)
                        {
                             for(int jr=0;jr<r.size();jr++)
                             {
                                int kkk=0;
                                for(int jc=0;jc<c.size();jc++)
                                {



                                    if((le2[jl]==c[jc].a) &&(r[jr]==c[jc].b ))
                                    {



                                        kkk=1;
                                        break;
                                    }
                                }
                                if(kkk==0)
                                {

                                   Y[tailledey]=le2[jl];
                                   tailledey++;
                                    for(int jp=(a[lcpi.a]+1);jp<=(a[lcpi.a]+lcpi.d);jp++)
                                    {

                                         Y[tailledey]=S[jp];
                                        tailledey++;

                                    }
                                    Y[tailledey]=r[jr];
                                    tailledey++;
                                    Y[tailledey]='\0';


                                        maw.push_back(Y);


                                    strcpy(Y,"");
                                    tailledey=0;
                                }
                             }
                        }

                    }

                }
                //
                i=lcpi.a;
            }
            if(l[k]>(P2.top().d)){


                 P2.push({i,0,l[k]});


            }
        }



        sort( maw.begin(), maw.end() );
        maw.erase( unique( maw.begin(), maw.end() ), maw.end() );




        string const nomFichier("result.txt");
        ofstream monFlux(nomFichier.c_str());

        if(monFlux)
        {
            monFlux << "The list of minimal absent words of human DNA with Pinho's algorithm" << endl;
        }
        else
        {
            cout << "ERROR: Could not open file" << endl;
        }


        for (int i = 0; i < (int)maw.size(); ++i)
        {
           monFlux<<maw[i]<<endl;
           info.insert(info.begin(),maw[i]);


        }
        monFlux.close();
        t2 = clock();

        temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    cout << temps << endl;

        info.insert(info.begin(),std::to_string(temps));
        info.insert(info.begin(),std::to_string(maw.size()));
        info.insert(info.begin(),std::to_string((float)TF*0.001));
        info.insert(info.begin(),std::to_string(NBRL));

        return info;
}



struct suffix
{
    int index; // To store original index
    int rank[2]; // To store ranks and next rank pair
};

// A comparison function used by sort() to compare two suffixes
// Compares two pairs, returns 1 if first pair is smaller
int cmp(struct suffix a, struct suffix b)
{
    return (a.rank[0] == b.rank[0])? (a.rank[1] < b.rank[1] ?1: 0):
        (a.rank[0] < b.rank[0] ?1: 0);
}

// This is the main function that takes a string 'txt' of size n as an
// argument, builds and return the suffix array for the given string
vector<int> buildSuffixArray(string txt, int n)
{
    // A structure to store suffixes and their indexes
    struct suffix suffixes[n];

    // Store suffixes and their indexes in an array of structures.
    // The structure is needed to sort the suffixes alphabatically
    // and maintain their old indexes while sorting
    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        suffixes[i].rank[0] = txt[i] - 'a';
        suffixes[i].rank[1] = ((i+1) < n)? (txt[i + 1] - 'a'): -1;
    }

    // Sort the suffixes using the comparison function
    // defined above.
    sort(suffixes, suffixes+n, cmp);

    // At his point, all suffixes are sorted according to first
    // 2 characters. Let us sort suffixes according to first 4
    // characters, then first 8 and so on
    int ind[n]; // This array is needed to get the index in suffixes[]
    // from original index. This mapping is needed to get
    // next suffix.
    for (int k = 4; k < 2*n; k = k*2)
    {
        // Assigning rank and index values to first suffix
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        // Assigning rank to suffixes
        for (int i = 1; i < n; i++)
        {
            // If first rank and next ranks are same as that of previous
            // suffix in array, assign the same new rank to this suffix
            if (suffixes[i].rank[0] == prev_rank &&
                    suffixes[i].rank[1] == suffixes[i-1].rank[1])
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = rank;
            }
            else // Otherwise increment rank and assign
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }

        // Assign next rank to every suffix
        for (int i = 0; i < n; i++)
        {
            int nextindex = suffixes[i].index + k/2;
            suffixes[i].rank[1] = (nextindex < n)?
                                suffixes[ind[nextindex]].rank[0]: -1;
        }

        // Sort the suffixes according to first k characters
        sort(suffixes, suffixes+n, cmp);
    }

    // Store indexes of all sorted suffixes in the suffix array
    vector<int>suffixArr;
    for (int i = 0; i < n; i++)
        suffixArr.push_back(suffixes[i].index);

    // Return the suffix array
    return suffixArr;
}

/* To construct and return LCP */
vector<int> kasai(string txt, vector<int> suffixArr)
{
    int n = suffixArr.size();

    // To store LCP array
    vector<int> lcp(n, 0);

    // An auxiliary array to store inverse of suffix array
    // elements. For example if suffixArr[0] is 5, the
    // invSuff[5] would store 0. This is used to get next
    // suffix string from suffix array.
    vector<int> invSuff(n, 0);

    // Fill values in invSuff[]
    for (int i=0; i < n; i++)
        invSuff[suffixArr[i]] = i;

    // Initialize length of previous LCP
    int k = 0;

    // Process all suffixes one by one starting from
    // first suffix in txt[]
    for (int i=0; i<n; i++)
    {
        /* If the current suffix is at n-1, then we don’t
        have next substring to consider. So lcp is not
        defined for this substring, we put zero. */
        if (invSuff[i] == n-1)
        {
            k = 0;
            continue;
        }

        /* j contains index of the next substring to
        be considered to compare with the present
        substring, i.e., next string in suffix array */
        int j = suffixArr[invSuff[i]+1];

        // Directly start matching from k'th index as
        // at-least k-1 characters will match
        while (i+k<n && j+k<n && txt[i+k]==txt[j+k])
            k++;

        lcp[invSuff[i]] = k; // lcp for the present suffix.

        // Deleting the starting character from the string.
        if (k>0)
            k--;
    }

    // return the constructed lcp array
    return lcp;
}
void printArr(vector<int>arr, int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

bool exists(std::string const & user_input,
            std::vector<std::string> const & words)
{
    for (int i = 0; i < (int)words.size(); i++)
        if (user_input == words[i])
            return true;
    return false;
}




vector<string> maw (string name , int tt)
{




    string su="";
    ifstream fichier(name);
    string s;
   if(fichier)
   {
      //L'ouverture s'est bien passée, on peut donc lire

      string ligne=""; //Une variable pour stocker les lignes lues

      while(getline(fichier, ligne)) //Tant qu'on n'est pas à la fin, on lit
      {
           if(ligne[0]!='>'){
         su=su+ligne;}
      }

   }
   else
   {
      cout << "ERROR: Could not open file for reading." << endl;
   }
    su+='$';
   for(int i =0; i<su.length();i++){
      s+=tolower(su[i]);
   }






    double start = gettime();
    start = clock();
    string str = s.substr(0, s.size()-1);
    vector<int> sa= buildSuffixArray(str, str.length());
    vector<int> lcp=kasai(str, sa);
    lcp.pop_back();
    lcp.insert(lcp.begin(),0);
    tt--;
    AhoCorasick AC;
    vector<string> queries;
    map<char,int> ci { {'a',0},{'c',1},{'g',2},{'t',3} };





    for(int i=0; i<sa.size(); i++)
    {
        if(string(s.begin() + sa[i], s.begin()+sa[i]+lcp[i]+1).length()==tt){
        queries.push_back(string(s.begin() + sa[i], s.begin()+sa[i]+lcp[i]+1)) ;
        queries.push_back(string(s.begin() + sa[i], s.begin()+sa[i]+lcp[i])) ;
        }


        if(i!=sa.size()-1){

            if(string(s.begin() + sa[i], s.begin()+sa[i]+lcp[i+1]+1).length()==tt){
            queries.push_back(string(s.begin() + sa[i], s.begin()+sa[i]+lcp[i+1]+1));
            queries.push_back(string(s.begin() + sa[i], s.begin()+sa[i]+lcp[i+1])) ;
            }
        }
    }
    vector<vector<int> > b1 (queries.size()/2,vector <int>(4,0)), b2 (queries.size()/2,vector <int>(4,0));


    AC.create_goto(queries);
    AC.find_matches(string(s));

    for (int i = 0; i < AC.positions.size(); i++)
    {
            for (list<int>::iterator itr = AC.positions[i].begin(); itr != AC.positions[i].end(); itr++){

                if(*itr-(queries[i].size()-1)!=0)
                    if(i%2==0)
                        b1[i/2][ci[s[(*itr-(queries[i].size()-1))-1]]]=1;
                    else
                        b2[i/2][ci[s[(*itr-(queries[i].size()-1))-1]]]=1;

            }


    }

    for(int i=0;i<queries.size();i++){
        for(int j=0;j<queries[i].length();j++){
                queries[i][j]=toupper(queries[i][j]);
            }
    }


    vector<string> T;

    for(int i=0;i<queries.size()/2;i++)
    {
        if(queries[2*i].find('$')==string::npos){
        if (b2[i][0]==1 and b1[i][0]!=1  && !exists(string(1,'A')+queries[2*i],T))
        {
            T.push_back(string(1,'A') + queries[2*i]);

        }
        if (b2[i][1]==1 and b1[i][1]!=1  && !exists(string(1,'C')+queries[2*i],T))
        {

            T.push_back(string(1,'C')+queries[2*i]);

        }        if (b2[i][2]==1 and b1[i][2]!=1 && !exists(string(1,'G')+queries[2*i],T))
        {

            T.push_back(string(1,'G')+queries[2*i]);

        }        if (b2[i][3]==1 and b1[i][3]!=1  && !exists(string(1,'T')+queries[2*i],T))
        {

            T.push_back(string(1,'T')+queries[2*i]);

        }


        if(i!=queries.size()/2-1)
        {

            if (b2[i][0]==1 and b1[i][0]!=1  && !exists(string(1,'A')+queries[2*i],T))
            {
            T.push_back(string(1,'A')+queries[2*i]);

            }
            if (b2[i][1]==1 and b1[i][1]!=1  && !exists(string(1,'C')+queries[2*i],T))
            {
            T.push_back(string(1,'C')+queries[2*i]);

            }
            if (b2[i][2]==1 and b1[i][2]!=1 && !exists(string(1,'G')+queries[2*i],T))
            {
            T.push_back(string(1,'G')+queries[2*i]);

            }        if (b2[i][3]==1 and b1[i][3]!=1  && !exists(string(1,'T')+queries[2*i],T))
            {
            T.push_back(string(1,'T')+queries[2*i]);

            }

        }

    }

    }
    double end = gettime();
    end = clock();

    T.push_back(to_string((end-start)/CLOCKS_PER_SEC));
    ofstream myfile;
  myfile.open ("result3.txt");
      myfile <<"The list of minimal absent words of human DNA with MAW's algorithm"<< endl;
     for(int i=0;i<T.size()-1;i++){
         myfile << T[i] << endl;
     }
    myfile.close();
    return T;


}


