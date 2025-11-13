using System;

namespace SmartMeterSystem
{
    public class Customer
    {
        public int CustomerId { get; set; }
        public string Name { get; set; }
        public string Address { get; set; }
        public SmartMeter Meter { get; set; } 

        public Customer(int id, string name, string address)
        {
            CustomerId = id;
            Name = name;
            Address = address;
        }
    }
}
