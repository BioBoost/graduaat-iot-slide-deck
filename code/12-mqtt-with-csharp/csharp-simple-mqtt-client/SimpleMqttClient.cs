using System;
using uPLibrary.Networking.M2Mqtt;

namespace NeedForSpeed
{
    public class SimpleMqttClient
    {
        private MqttClient client = null;

        public void Connect(string broker)
        {
            // Zorg dat er zeker geen connectie open staat
            Disconnect();

            // Maak een nieuwe client en connecteer met de broker
            client = new MqttClient(broker);
            client.Connect(Guid.NewGuid().ToString());
        }

        public bool IsConnected()
        {
            // Als er geen client is,
            // kan er ook geen connectie zijn
            if (client == null)
            {
                return false;
            }

            // Geef de staat van de huidige connectie terug
            return client.IsConnected;
        }

        public void Disconnect()
        {
            if (client != null)
            {
                client.Disconnect();
            }
        }

    }
}
