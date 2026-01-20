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
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        public Contacts()
        {
            InitializeComponent();
            ViewContact();
        }
        private async void ViewContact()
        {
            RestRequest ContactRequest = new RestRequest("/Users", Method.Get);
            RestResponse ContactResponse = await restClient.ExecuteAsync(ContactRequest);
            if (ContactResponse.IsSuccessful)
            {
                var data = ContactResponse.Content;
                MessageBox.Show(data);
                
            }
        }
    }
}
