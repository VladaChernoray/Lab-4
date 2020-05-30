#include <iostream>
#include <cmath>

using namespace std;
char *expression = "cos 0 / 2.5 + (sin(x) * 3 - b)";

enum OPERATION
{
    NON, NUM, VAL, PLS, MNS, MUL, DIV, SIN, COS, LFT, RGT
};

struct node
{
    OPERATION math_actions = NON;
    float num = 0;
    char one;
    node* up = nullptr;
    node* l = nullptr; //left
    node* r = nullptr; //right

    const char* message = "Please enter a variable: ";

    node(node* parent)
    {
        up = parent;
    }

    float res(){
        switch (math_actions) {
            case NUM: 
                return num;
            case PLS: 
                return l->res() + r->res();
            case MNS: 
                return l->res() - r->res();
            case MUL:
                return l->res() * r->res();
            case DIV: 
                return l->res() / r->res();
            case SIN: 
                return sin(r->res());
            case COS: 
                return cos(r->res());
            case VAL:
                cout << message << one << endl;

                cin >> num;
                return num;
            default:
                float res = 0;
                if(l != nullptr) res += l->res();
                if(r != nullptr) res += r->res();
                return res;
        }
    }
};

struct syntax_tree
{
    node* head = new node(nullptr);

    void put(OPERATION math_actions)
    {
        if(math_actions == LFT) l_bracket();
        else if(math_actions == RGT) r_bracket();
        else if(head-> math_actions== NON)
        {

            if (head->l == nullptr)
            {
                head->l = new node(head);
                head->l-> math_actions = NUM;
            }

            head-> math_actions = math_actions;
            head->r = new node(head);
            head = head->r;
        }
        else
        {
            if((math_actions == MUL || math_actions == DIV) && (head->math_actions == PLS || head->math_actions == MNS))
            {
                head->r->up = new node(head);
                head->r->up->l = head->r;
                head->r = head->r->up;
                head = head->r;
            }
            else
            {
                if(head->up == nullptr)
                {
                    head->up = new node(nullptr);
                    head->up->l = head;
                    head = head->up;
                } 
                else
                {
                    if(head->up->l == head)
                    {
                        head->up = new node(head->up);
                        head->up->l = head;
                        head->up->up->l = head->up;
                        head = head->up;
                    }
                    else
                    {
                        head->up = new node(head->up);
                        head->up->l = head;
                        head->up->up->r = head->up;
                        head = head->up;
                    }
                }
            }
            head->math_actions = math_actions;
            head->r = new node(head);
            head = head->r;
        }
    }

    void put(float num)
    {
        number(num);
    }

    void put(char one)
    {
        val(one);
    }

    float result()
    {
        while (head->up != nullptr) head = head->up;
        return head->result();
    }

private:
    void lbracket()
    {
        head->l = new node(head);
        head = head->l;
    }

    void rbracket()
    {
        if(head->up != nullptr) head = head->up;
    }

    void number(float num)
    {
        head->math_actions = NUM;
        head->num = num;
        head = head->up;
    }

    void val(char one)
    {
        head->math_actions = VAL;
        head->one = one;
        head = head->up;
    }
};

class parser{
    syntax_tree syntax_tree;
public:
    void parse(char arr[])
    {
        int i = 0;

        syntax_tree.put(LFT);
        syntax_tree.put(LFT);

        while(arr[i] != '\0')
        {
            if(arr[i] == ' '){}
            else if(arr[i] == '+')
                syntax_tree.put(PLS);
            else if(arr[i] == '-') 
                syntax_tree.put(MNS);
            else if(arr[i] == '*') 
                syntax_tree.put(MUL);
            else if(arr[i] == '/') 
                syntax_tree.put(DIV);
            else if(arr[i] == '(')
                syntax_tree.put(LFT);
            else if(arr[i] == ')') 
                syntax_tree.put(RGT);
            else if(arr[i] == 'S' || arr[i] == 's')
            {
                syntax_tree.put(SIN);
                i += 2;
            }
            else if(arr[i] == 'C' || arr[i] == 'c')
            {
                syntax_tree.put(COS);
                i += 2;
            } 
            else if((arr[i] >= 48 && arr[i] <= 57) || arr[i] == 44 || arr[i] == 46)
            {
                int a = 0;
                float num = 0;

                while (arr[i + a] >= 48 && arr[i + a] <= 57)
                {
                    a++;
                }

                while ((arr[i] >= 48 && arr[i] <= 57) || arr[i] == 44 || arr[i] == 46)
                {
                    if(a > 0) num += (arr[i] - 48) * pow(10, a-1);
                    else if(a < 0) num += (arr[i] - 48) * pow(10, a);
                    a--;
                    i++;
                }
                syntax_tree.put(num);
                i--;
            }
            else
            {
                syntax_tree.put(arr[i]);
            }
            i++;
        }
        syntax_tree.put(RGT);
    }

    float result()
    {
        return syntax_tree.result();
    }
};

int main() {
    parser parser;
    parser.parse(expression);
    cout << parser.result();
    return 0;
}
