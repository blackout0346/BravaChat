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
    /// Логика взаимодействия для Contacts.xaml
    /// </summary>
    public partial class Contacts : Window
    {
  
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        private int UserId;
        public class UserDto
        {
            [JsonPropertyName("Login")]
            public string Login { get; set; }
        }
        public class ContactDto
        {
            [JsonPropertyName("Login")]
            public string Login { get; set; }
            [JsonPropertyName("Id")]
            public int Id { get; set; }
        }
        public class ContactsResponse
        {
            [JsonPropertyName("Contact")]
            public List<ContactDto> ContactDto { get; set; }
        }

        public Contacts(int userId)
        {
            InitializeComponent();

            UserId = userId;
            LoadData();
        }
        private async void LoadData()
        {
            await GetCurrentUserName();
            await ViewContact();
        }
        private async Task GetCurrentUserName()
        {
            RestRequest request = new RestRequest($"/users/{UserId}", Method.Get);
            var response = await restClient.ExecuteAsync(request); // Выполняем без <UserDto> сначала

            if (response.IsSuccessStatusCode)
            {

                var data = System.Text.Json.JsonSerializer.Deserialize<UserDto>(response.Content);
                    UserName.Content = data.Login;
            }
            else
            {
                UserName.Content = "Ошибка запроса: " + response.StatusCode;
            }
        }
        private async Task ViewContact()
        {

            RestRequest ContactRequest = new RestRequest($"/contacts/{UserId}", Method.Get);

            var ContactResponse = await restClient.ExecuteAsync<ContactsResponse>(ContactRequest);

       
            if (ContactResponse.IsSuccessStatusCode && ContactResponse.Data != null && ContactResponse.Data.ContactDto != null)
            {
                BoxContact.Items.Clear();

                foreach (var contactData in ContactResponse.Data.ContactDto)
                {
                    ItemContact itemContact = new ItemContact(contactData.Id,contactData.Login);
                    BoxContact.Items.Add(itemContact);
                }
            }
            else
            {
                MessageBox.Show("Не удалось загрузить контакты");
                return;
            }
        }

      

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            MainWindow Logins = new MainWindow();
            Logins.Show();
            Close();
        }

        private void CreateChat_Click(object sender, RoutedEventArgs e)
        {
             AddUsers addUsers = new AddUsers();
             addUsers.Show();
        }

        private void BoxContact_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
       
            if (BoxContact.SelectedItem is ItemContact item)
            {
                int idforChat = item.ContactId;
                UserControl1 chat = new UserControl1(this.UserId, idforChat);
                ChatDisplay.Content = chat;
            }
            else
            {
                // Если попали сюда — значит тип данных не совпал
                MessageBox.Show("Выбранный элемент не является контактом");
            }
        }
    }
}
