

using System;

class Program
{
    static void Main()
    {
        Student[] students = new Student[5];

        Console.WriteLine("Enter details for 5 students:\n");

        for (int i = 0; i < 5; i++)
        {
            Console.WriteLine($"\n--- Student {i + 1} ---");
            students[i] = new Student();
            students[i].InputData();
            students[i].Calculate();
        }

        Console.WriteLine("\n=== Student Report ===");

        foreach (Student s in students)
        {
            s.Display();
        }

        Console.ReadLine();
    }
}
