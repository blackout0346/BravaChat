using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Web.UI.WebControls;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using static RestApi.Contacts;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.StartPanel;

namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для AddUsers.xaml
    /// </summary>
    public partial class AddUsers : Window
    {
        class ViewUsers
        {
            [JsonPropertyName("Login")]
            public string login { get; set; }
            [JsonPropertyName("Id")]
            public int Id { get; set; }
        }
        private int MyId;
        private string SearchInput;

        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        public AddUsers(int myId)
        {
            InitializeComponent();
            MyId = myId;
            search.TextChanged += (s, e) =>
            {
                SearchInput = search.Text;
                GetUsersFromDB();
            };
            SearchInput = "";

        
        }
        private async void GetUsersFromDB()
        {
            if (string.IsNullOrEmpty(SearchInput)) return;
            try
            {
                string token = Properties.Settings.Default.SavedToken;
                RestRequest request = new RestRequest($"/users/search/{SearchInput}", Method.Get);
                request.AddHeader("Authorization", "Bearer " + token);
                var response = await restClient.ExecuteAsync<List<ViewUsers>>(request);
                if (response.IsSuccessStatusCode)
                {
                    ListUsers.Items.Clear();
                    foreach (var item in response.Data)
                    {
                        if (item.Id == MyId) continue;
                     
                        AddItemUser addItemUser = new AddItemUser(item.Id, item.login);
                        addItemUser.OnaddUserClicked += async (PartnerId) =>
                        {
                            await AddFriend(PartnerId);
                           
                        };
                        if (MyId == addItemUser.PartnerId)
                        {
                            continue;
                        }
                        ListUsers.Items.Add(addItemUser);
                    }
                }
                else
                {
                    MessageBox.Show($"error {response.Content}");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"catch:{ex.Message}");
            }
        }
        private async Task AddFriend(int partnerId)
        {
            var request = new RestRequest("/contacts/add", Method.Post);
            request.AddJsonBody(new { userId1 = MyId, userId2 = partnerId });
            var response = await restClient.ExecuteAsync(request);
           
            if (response.IsSuccessStatusCode)
            {
                MessageBox.Show("контакт добавлен!");

            }
            else
            {
                MessageBox.Show($"Ошибка :{response.Content}");
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
