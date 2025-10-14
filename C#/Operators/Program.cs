namespace Operators
{
    internal class Program
    {
        static void Main(string[] args)
        {
            int a = 2;
            int b = ++a;
            string Result;
          Console.WriteLine("The value of a:" +a); 
          Console.WriteLine("The value of b:" +b);
           Result = (a % 2) != 0 ? "a" : "b";
           Console.WriteLine("Result:" + Result);
        }
    }
}
