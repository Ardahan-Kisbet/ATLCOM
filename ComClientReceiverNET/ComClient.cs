using System;
using System.Runtime.InteropServices;
using ATLCOMLib;

namespace ComClientReceiverNET
{
    class ComClient : IDisposable
    {
        public MyEventObject com;

        public ComClient()
        {
            com = new MyEventObject();
            com.FireEventTest += FireEventTest;
        }

        internal void TransmitData()
        {
            // we don't transmit anything though the idea is faking some transmit/sender method which is responsible to fire event inside com server
            // in this case SimpleEvent function will fire an event from com server and will invoke the clients/listeners
            com.SimpleEvent();
        }

        static void FireEventTest(double receivedVal)
        {
            Console.WriteLine(
                Environment.NewLine
                + "received value: " + receivedVal
                + Environment.NewLine);
        }

        void IDisposable.Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                com.FireEventTest -= FireEventTest;
                Marshal.FinalReleaseComObject(com);
            }
        }
    }
}
