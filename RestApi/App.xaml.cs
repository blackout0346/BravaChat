using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для App.xaml
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);
            string token = RestApi.Properties.Settings.Default.SavedToken;
            int UserId = RestApi.Properties.Settings.Default.SavedUserId;
            if (!string.IsNullOrEmpty(token) && UserId > 0)
            {
                Contacts contacts = new Contacts(UserId);
                contacts.Show();
            }
            else
            {
                MainWindow loginWindow = new MainWindow();
                loginWindow.Show();
            }

        }
    }
}
