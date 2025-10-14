using System.Numerics;
using System.Runtime.ExceptionServices;

namespace Datatype_demo
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");
            byte byte_variable = 5;
            sbyte sbyte_variable = -44;
            short short_variable = 10000;
            ushort ushort_variable = 50000;
            int int_variable = -155;
            uint uint_variable = 66;
            long long_variable = 5000000;
            ulong ulong_variable = 20000;
            float float_variable = 45.25f;
            double double_variable = 20.55;
            Console.WriteLine("Byte Variable:" + byte_variable);
            Console.WriteLine("Sbyte Variable:" + sbyte_variable);
            Console.WriteLine("Short_variable:" + short_variable);
            Console.WriteLine("Ushort_variable:" + ushort_variable);
            Console.WriteLine("Int_variable:" + int_variable);
            Console.WriteLine("Unit_varible:" + uint_variable);
            Console.WriteLine("Long_variable:" + long_variable);
            Console.WriteLine("Ulong_variable:" + ulong_variable);
            Console.WriteLine("Float_variable:" + float_variable);
            Console.WriteLine("Double_variable:" + double_variable);



            /* calling statement */
            int result = addition(100, 200);
            int result1 = addition(1000, 2000);
            Console.WriteLine("Result:"  +result);
            Console.WriteLine("Result1:" +result1);
            
       

            
         int first =Convert.ToInt32(Console.ReadLine());
         double second = Convert.ToDouble(Console.ReadLine());
         double result3 = addition(first, second);


         

        }
           /* method defination  */
           static public int addition(int first, int second)
        {
            int sum = first + second;
            return sum;
        }


        static public double addition(int first, double second)
        {
            double sum = first + second;
            return sum;
        }
        

    }
}
