using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.UI.WebControls;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для ItemContact.xaml
    /// </summary>
    public partial class ItemContact : UserControl
    {
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        private string contactName;
        struct UserNames
        {
            public string Name { get; set; }
        }

        public ItemContact(string item)
        {
            contactName = item;
            InitializeComponent();
            SelectUsers(contactName);
        }

        public async void SelectUsers(string contactname)
        {
            RestRequest ContactRequest = new RestRequest("/Contact", Method.Get);
            UserNames userNames = new UserNames() { Name = restClient.GetAsync(ContactRequest).ToString() };

            RestResponse ContactResponse = await restClient.ExecuteAsync<UserNames>(ContactRequest);
            if (ContactResponse.IsSuccessStatusCode)
            {
                UserName.Content = ContactResponse.Content;
            }
        }
    }
}
