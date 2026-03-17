using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
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
    /// Логика взаимодействия для AddGroups.xaml
    /// </summary>
    public partial class AddGroups : Window
    {
        class ViewUsers
        {
            [JsonPropertyName("Login")]
            public string login { get; set; }
            [JsonPropertyName("Id")]
            public int Id { get; set; }
        }



     
        class Group
        {
            [JsonPropertyName("name")]
            public string Name { get; set; }
            [JsonPropertyName("UserIds")]
            public List<int> UserIds { get; set; }

        }
        private int MyId;
        private string namegroup;
        public List<int> SelectedUserIds = new List<int>();
        private string searchinput;
        RestClient restClient = new RestClient("http://127.0.0.1:18080");

        public AddGroups(int myId)
        {
            InitializeComponent();
            MyId = myId;
            search.TextChanged += (s, e) => FriendsFromDB();

            {
            accept.Click += accept_Click;
            }


        }
        private async void FriendsFromDB()
        {
            if (string.IsNullOrEmpty(search.Text)) return;
            try
            {
                string token = Properties.Settings.Default.SavedToken;
                RestRequest request = new RestRequest($"/users/search/{search.Text}", Method.Get);
                request.AddHeader("Authorization", "Bearer " + token);
                var response = await restClient.ExecuteAsync<List<ViewUsers>>(request);
                if (response.IsSuccessStatusCode && response.Data != null)
                {
                    ListUsers.Items.Clear();
                    foreach (var item in response.Data)
                    {
                       
                        ItemInGroups addItemGroups = new ItemInGroups(item.Id, item.login);
                        addItemGroups.OnUserChecked += (PartnerId) =>
                        {
                            if (!SelectedUserIds.Contains(PartnerId))
                            {
                                SelectedUserIds.Add(PartnerId);
                            }
                        };
                        addItemGroups.OnUserUnChecked += (PartnerId) =>
                        {
                            if (SelectedUserIds.Contains(PartnerId))
                            {
                                SelectedUserIds.Remove(PartnerId);
                            }
                        };
                        ListUsers.Items.Add(addItemGroups);
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

        private async void CreateGroup(string name)
        {
            RestRequest request = new RestRequest("/chats", Method.Post);
            var creategroup = new Group
            {
                Name = name,
                UserIds = SelectedUserIds
            };
            request.AddJsonBody(creategroup);
            var resource = await restClient.ExecuteAsync(request);
            if (resource.IsSuccessStatusCode)
            {
                MessageBox.Show("Группа успешно создана!");
            }
            
        }

        private void accept_Click(object sender, RoutedEventArgs e)
        {
            string GroupName = NameGroup.Text;
            if (string.IsNullOrEmpty(GroupName))
            {
                MessageBox.Show("Введите название группы");
                return;
            };
            if(SelectedUserIds.Count == 0)
            {
                MessageBox.Show("Добавьте хотя бы одного друга");
                return;
            };
            if(!SelectedUserIds.Contains(MyId))
            {
                SelectedUserIds.Add(MyId);
            };

            CreateGroup(GroupName);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
