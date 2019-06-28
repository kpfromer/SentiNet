
void ((*create_transition)(int))(int a)
{
  void (*func)(int b)
  {
    return b + a;
  }
  return func;
}

int main()
{
  int i = 3;
  (create_transition(i))(4);
  return 0;

}
