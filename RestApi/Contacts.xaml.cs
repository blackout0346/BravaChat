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
 
        bool isclose = false;
        public class UserDto
        {
            [JsonPropertyName("Login")]
            public string Login { get; set; }
        }
        public class ContactDto
        {
            [JsonPropertyName("chatId")]
            public int ChatId { get; set; }

            [JsonPropertyName("partnerId")] 
            public int PartnerId { get; set; }

            [JsonPropertyName("login")]
            public string Login { get; set; }
        
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
            var response = await restClient.ExecuteAsync(request); 

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

            var ContactResponse = await restClient.ExecuteAsync<List<ContactDto>>(ContactRequest);

       
            if (ContactResponse.IsSuccessStatusCode && ContactResponse.Data != null && ContactResponse.Data != null)
            {
                BoxContact.Items.Clear();

                foreach (var contactData in ContactResponse.Data)
                {
                    ItemContact itemContact = new ItemContact(contactData.ChatId,contactData.Login);
                    BoxContact.Items.Add(itemContact);
                }
            }
            else
            {
                MessageBox.Show("Не удалось загрузить контакты. Код: " + ContactResponse.StatusCode);
                return;
            }
        }
        private async void GetContact()
        {
            try
            {
                var request = new RestRequest($"/contacts/{this.UserId}", Method.Get);
                var response = await restClient.ExecuteAsync<List<ContactDto>>(request);
                if(response.IsSuccessStatusCode && response.Data != null)
                {
                    BoxContact.Items.Clear();
                    foreach(var contactData in response.Data)
                    {
                        ItemContact item = new ItemContact(contactData.ChatId, contactData.Login);
                        BoxContact.Items.Add(item);
                    }
                }
            }
            catch (Exception ex)
            {
            }
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            MainWindow Logins = new MainWindow();
            Logins.Show();
            Close();
        }



        private void BoxContact_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
       
            if (BoxContact.SelectedItem is ItemContact item)
            {
                int chatId = item.chatId;
                UserControl1 chat = new UserControl1(this.UserId, chatId);
                ChatDisplay.Content = chat;
            }
            else
            {
                
                MessageBox.Show("Выбранный элемент не является контактом");
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
          
            //check();
     
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            AddUsers addUsers = new AddUsers(this.UserId);
            addUsers.Closed += (s, args) =>
            {
                GetContact();
            };
            addUsers.Show();
        }
    }
}
