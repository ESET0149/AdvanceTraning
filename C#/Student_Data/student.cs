
using System;

public class Student
{
    public string Name;
    public int[] Marks = new int[3];
    public int Total;
    public float Average;
    public char Grade;

    public void InputData()
    {
        Console.Write("Enter Student Name: ");
        Name = Console.ReadLine();

        for (int i = 0; i < 3; i++)
        {
            Console.Write($"Enter Mark {i + 1}:");
            Marks[i] = Convert.ToInt32(Console.ReadLine());
        }
    }

    public void Calculate()
    {
        Total = 0;
        foreach (int mark in Marks)
        {
            Total += mark;
        }

        Average = Total / 3.0f;

        if (Average >= 90)
            Grade = 'A';
        else if (Average >= 80)
            Grade = 'B';
        else if (Average >= 70)
            Grade = 'C';
        else if (Average >= 60)
            Grade = 'D';
        else
            Grade = 'F';
    }

    public void Display()
    {
        Console.WriteLine($"\nName: {Name}");
        Console.WriteLine($"Total Marks: {Total}");
        Console.WriteLine($"Average: {Average:F2}");
        Console.WriteLine($"Grade: {Grade}"};
}
