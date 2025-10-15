using System;
using System.Globalization;
using System.Text.RegularExpressions;

namespace StringOperations
{
    class Program
    {
        static void Main()
        {
            if (!VerifyCredentials())
            {
                Console.WriteLine("Invalid userId and password");
                return;
            }

            Console.WriteLine("\nLogin successful!\n");

            Console.WriteLine("Enter a paragraph:");
            string paragraph = Console.ReadLine();

            Console.WriteLine($"\n1. Length of paragraph: {GetLength(paragraph)}");
            Console.WriteLine($"2. Number of words: {GetWordCount(paragraph)}");
            Console.WriteLine($"3. Number of spaces: {GetSpaceCount(paragraph)}");

            Console.WriteLine("4. Sentences:");
            ListSentences(paragraph);

            Console.WriteLine($"5. First occurrence of 'a': {GetFirstIndexOfA(paragraph)}");

            Console.WriteLine("6. 'student' to uppercase:");
            Console.WriteLine(ConvertStudentToUpper(paragraph));

            Console.WriteLine("7. Proper case of 'I am A Student':");
            Console.WriteLine(ToProperCase("I am A Student"));

            Console.WriteLine($"8. Total number of 'a': {CountLetterA(paragraph)}");

            Console.WriteLine("\nEnter a vehicle number plate (e.g., KA 19 MC 1234):");
            string plate = Console.ReadLine().ToUpper();
            Console.WriteLine("9. State for vehicle number: " + GetStateFromCode(plate));
        }

        static bool VerifyCredentials()
        {
            string defaultUsername = "admin";
            string defaultPassword = "12345";

            Console.WriteLine("Enter the username:");
            string username = Console.ReadLine();

            Console.WriteLine("Enter the password:");
            string password = Console.ReadLine();

            return username == defaultUsername && password == defaultPassword;
        }

        static int GetLength(string input)
        {
            return input.Length;
        }

        static int GetWordCount(string input)
        {
            string[] words = input.Split(new char[] { ' ', '\n', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            return words.Length;
        }

        static int GetSpaceCount(string input)
        {
            int count = 0;
            foreach (char c in input)
            {
                if (c == ' ') count++;
            }
            return count;
        }

        static void ListSentences(string input)
        {
            string[] sentences = input.Split(new char[] { '.', '?', '!' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (string sentence in sentences)
            {
                Console.WriteLine("- " + sentence.Trim());
            }
        }

        static int GetFirstIndexOfA(string input)
        {
            return input.IndexOf('a');
        }

        static string ConvertStudentToUpper(string input)
        {
            return Regex.Replace(input, "student", "STUDENT", RegexOptions.IgnoreCase);
        }

        static string ToProperCase(string input)
        {
            TextInfo textInfo = CultureInfo.CurrentCulture.TextInfo;
            return textInfo.ToTitleCase(input.ToLower());
        }

        static int CountLetterA(string input)
        {
            int count = 0;
            foreach (char c in input.ToLower())
            {
                if (c == 'a') count++;
            }
            return count;
        }

        static string GetStateFromCode(string numberPlate)
        {
            if (numberPlate.Length < 2)
                return "Invalid number plate";

            string code = numberPlate.Substring(0, 2);
            switch (code)
            {
                case "KA": return "Karnataka";
                case "MH": return "Maharashtra";
                case "DL": return "Delhi";
                default: return "Unknown state code";
            }
        }
    }
}
