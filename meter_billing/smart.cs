using System.Collections.Generic;

namespace SmartMeterSystem
{
    public class SmartMeter
    {
        public int MeterSerialNo { get; set; }
        public string Model { get; set; }
        public List<double> Readings { get; set; } = new List<double>();

        public SmartMeter(int serialNo, string model)
        {
            MeterSerialNo = serialNo;
            Model = model;
        }

        public void AddReading(double reading)
        {
            if (reading >= 0)
                Readings.Add(reading);
        }

        public double GetTotalConsumption()
        {
            double total = 0;
            foreach (var r in Readings)
                total += r;
            return total;
        }
    }
}
