using System;

namespace Datatype_demo
{
    internal class Program
    {
        static string[] name = new string[3];
        static int[] maths = new int[3];
        static int[] english = new int[3];
        static int[] science = new int[3];
        static int[] total = new int[3];
        static double[] average = new double[3];
       static string[] Grade = new string[3];
       


        static void Main(string[] args)
        {
            Console.WriteLine("Enter First Student Details");
            getStudentData(0);
            getTotal(0);
            getAverage(0);
            Grade[0] = "N/A";

            Console.WriteLine("\nEnter Second Student Details");
            getStudentData(1);
            getTotal(1);
            getAverage(1);
            Grade[1] = "N/A";

            Console.WriteLine("\nEnter Third Student Details");
            getStudentData(2);
            getTotal(2);
            getAverage(2);
            Grade[2] = "N/A";

            // Display all 3 students together
            Console.WriteLine("\n-------------------- Student Details --------------------");
            Console.WriteLine("Name\tMaths\tEnglish\tScience\tTotal\tAverage\tGrade");
            Console.WriteLine("----------------------------------------------------------");

            Display(0);
            Display(1);
            Display(2);

            Console.WriteLine("----------------------------------------------------------");
        }

        private static void getStudentData(int index)
        {
            Console.Write("Enter name: ");
            name[index] = Console.ReadLine();

            Console.Write("Enter Maths marks: ");
            maths[index] = Convert.ToInt32(Console.ReadLine());

            Console.Write("Enter English marks: ");
            english[index] = Convert.ToInt32(Console.ReadLine());

            Console.Write("Enter Science marks: ");
            science[index] = Convert.ToInt32(Console.ReadLine());
        }

        private static void getTotal(int index)
        {
            total[index] = maths[index] + english[index] + science[index];
        }

        private static void getAverage(int index)
        {
            average[index] = total[index] / 3.0;
        }
        
        private static void Display(int index)
        {
            Console.WriteLine($"{name[index]}\t{maths[index]}\t{english[index]}\t{science[index]}\t{total[index]}\t{average[index]:F2}\t{Grade[index]}");
        }
    }
}
