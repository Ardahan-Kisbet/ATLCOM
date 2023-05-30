using System;
using System.Threading;

namespace ComClientReceiverNET
{
    class Program
    {
        static void Main(string[] args)
        {
            const bool doTransmit = false;

            using (var client = new ComClient())
            {
                Run(client, doTransmit);
            }
        }

        static void Run(ComClient client, bool doTransmit)
        {
            var isRunning = true;
            var lastTransmit = DateTime.MinValue;
            var transmitInterval = TimeSpan.FromSeconds(1);

            while (isRunning)
            {
                if (doTransmit)
                {
                    var now = DateTime.UtcNow;
                    var timeSinceLastTransmit = (now - lastTransmit);

                    if (timeSinceLastTransmit > transmitInterval)
                    {
                        client.TransmitData();
                        Console.WriteLine(Environment.NewLine);

                        lastTransmit = now;
                    }
                }

                Thread.Sleep(TimeSpan.FromMilliseconds(10));

                if (Console.KeyAvailable
                    && Console.ReadKey().Key == ConsoleKey.Q)
                {
                    isRunning = false;
                }
            }
        }
    }
}
