using System;
using System.Collections.Generic;

namespace SmartMeterSystem
{
    class Program
    {
        static Dictionary<int, Customer> customers = new Dictionary<int, Customer>();
        static Dictionary<int, SmartMeter> meters = new Dictionary<int, SmartMeter>();
        const double RATE_PER_UNIT = 0.15; 

        static void Main(string[] args)
        {
            Console.WriteLine("------ Welcome to Smart Meter System --------");

            while (true)
            {
                ShowMenu();
                Console.Write("Enter your choice: ");
                string choice = Console.ReadLine();

                switch (choice)
                {
                    case "1":
                        AddCustomer();
                        break;
                    case "2":
                        AddSmartMeter();
                        break;
                    case "3":
                        MapMeter();
                        break;
                    case "4":
                        AddReading();
                        break;
                    case "5":
                        GenerateBill();
                        break;
                    case "6":
                        Console.WriteLine("Terminating......");
                        return;
                    default:
                        Console.WriteLine("Invalid choice. Try again.");
                        break;
                }

                Console.WriteLine("-------------------------------------------");
            }
        }

        static void ShowMenu()
        {
            Console.WriteLine("\n1. Add Customer");
            Console.WriteLine("2. Add Smart Meter");
            Console.WriteLine("3. Map Meter");
            Console.WriteLine("4. Add Reading");
            Console.WriteLine("5. Generate Bill");
            Console.WriteLine("6. Exit");
        }

        static void AddCustomer()
        {
            Console.Write("Enter Name: ");
            string name = Console.ReadLine();

            Console.Write("Enter Customer ID: ");
            int id = int.Parse(Console.ReadLine());

            Console.Write("Enter Address: ");
            string address = Console.ReadLine();

            if (customers.ContainsKey(id))
            {
                Console.WriteLine("Customer ID already exists.");
                return;
            }

            customers[id] = new Customer(id, name, address);
            Console.WriteLine("Customer Data added successfully.");
        }

        static void AddSmartMeter()
        {
            Console.Write("Enter Meter Serial No: ");
            int serial = int.Parse(Console.ReadLine());

            Console.Write("Enter Model: ");
            string model = Console.ReadLine();

            if (meters.ContainsKey(serial))
            {
                Console.WriteLine("Meter already exists.");
                return;
            }

            meters[serial] = new SmartMeter(serial, model);
            Console.WriteLine("Meter Data added successfully.");
        }

        static void MapMeter()
        {
            Console.Write("Enter Customer ID: ");
            int custId = int.Parse(Console.ReadLine());

            if (!customers.ContainsKey(custId))
            {
                Console.WriteLine("Customer not found.");
                return;
            }

            Console.Write("Enter Meter Serial No: ");
            int meterId = int.Parse(Console.ReadLine());

            if (!meters.ContainsKey(meterId))
            {
                Console.WriteLine("Meter not found.");
                return;
            }

            customers[custId].Meter = meters[meterId];
            Console.WriteLine("Customer mapped with meter.");
        }

        static void AddReading()
        {
            Console.Write("Enter Meter Serial No: ");
            int meterId = int.Parse(Console.ReadLine());

            if (!meters.ContainsKey(meterId))
            {
                Console.WriteLine("Meter not found.");
                return;
            }

            Console.Write("Enter reading: ");
            double reading = double.Parse(Console.ReadLine());

            meters[meterId].AddReading(reading);
            Console.WriteLine("Reading received successfully.");
        }

        static void GenerateBill()
        {
            Console.Write("Enter Customer ID: ");
            int custId = int.Parse(Console.ReadLine());

            if (!customers.ContainsKey(custId))
            {
                Console.WriteLine("Customer not found.");
                return;
            }

            Customer customer = customers[custId];
            if (customer.Meter == null)
            {
                Console.WriteLine("No meter mapped to this customer.");
                return;
            }

            double total = customer.Meter.GetTotalConsumption();
            double bill = total * RATE_PER_UNIT;

            Console.WriteLine("\nYour current bill details are as follows:");
            Console.WriteLine($"Customer ID         : {customer.CustomerId}");
            Console.WriteLine($"Meter ID            : {customer.Meter.MeterSerialNo}");
            Console.WriteLine($"Total units consumed: {total} kwpH");
            Console.WriteLine($"Total Bill          : ${bill:F2}");
        }
    }
}
