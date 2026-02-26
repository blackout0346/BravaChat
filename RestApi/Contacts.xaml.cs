using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace RestApi
{
   
    /// <summary>
    /// Логика взаимодействия для Contacts.xaml
    /// </summary>
    public partial class Contacts : Window
    {
        ItemContact contact;
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        private object UserId = null;
        struct UserNames
        {
            public string Name { get; set; }
        }

        public Contacts(object userId)
        {
            InitializeComponent();
            ViewContact();
            UserId = userId;
            UserName.Content = UserId;
        }

        private async void ViewContact()
        {
            RestRequest ContactRequest = new RestRequest("/contacts", Method.Get);
            UserNames userNames = new UserNames() { Name = restClient.GetAsync(ContactRequest).ToString() };

            RestResponse ContactResponse = await restClient.ExecuteAsync<UserNames>(ContactRequest);
            if (ContactResponse.IsSuccessStatusCode)
            {
                foreach (var item in userNames.ToString())
                {
                    contact = new ItemContact(item.ToString());
                }
            }
        }

      

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            MainWindow Logins = new MainWindow();
            Logins.Show();
            Close();
        }
    }
}
