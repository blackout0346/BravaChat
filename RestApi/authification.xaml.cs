using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
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
using System.Xml.Linq;
using System.Text.Json.Serialization;
namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для authification.xaml
    /// </summary>
    public partial class authification : Window
    {
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        public class AuthResponses
        {
            [JsonPropertyName("Token")]
            public string Token { get; set; }

            [JsonPropertyName("Id")]
            public int Id { get; set; }


        }
        class AuthificationData
        {
            [JsonPropertyName("Login")]
            public string Login { get; set; }

            [JsonPropertyName("emails")]
            public string emails { get; set; }

            [JsonPropertyName("passwords")]
            public string passwords { get; set; }

            [JsonPropertyName("numbers")]
            public string numbers { get; set; }

            [JsonPropertyName("photo")]
            public string photo { get; set; }
        }
        private string Login = null;
        private string emails = null;
        private string passwords = null;
        private string numbers = null;

        
        public authification()
        {
            InitializeComponent();
    

        }

        private void setData()
        {
            Login = inputName.Text;
            emails = inputEmail.Text.Trim();
            passwords = inputPassword.Password.Trim();
            numbers = inputNumber.Text.Trim();
       
          
            if (numbers.Length < 11)
            {
                MessageBox.Show("Некорректный номер");
                return;
            }
            if (!string.IsNullOrWhiteSpace(Login) && !string.IsNullOrWhiteSpace(emails) && !string.IsNullOrWhiteSpace(passwords))
            {
                AuthResponse(Login, emails, passwords, numbers);
                inputName.Text = " ";
                passwords = " ";
                inputEmail.Text = " ";
                inputNumber.Text = " ";
                return;
            }
            else
            {
               
                MessageBox.Show("Введите все поля");
                inputName.Text = " ";
                passwords = " ";
                inputEmail.Text = " ";
                inputNumber.Text = " ";
                return;
            }

            
        }

        private async void AuthResponse(string name, string email, string password, string number)
        {

            var NewAuthUser = new AuthificationData
            {
                Login = name,
                emails = email,
                passwords = password,
                numbers = number,
                photo = ""

            };
    
            RestRequest AuthRequest = new RestRequest("/auth", Method.Post);
           
            AuthRequest.AddJsonBody(NewAuthUser);
            var response = await restClient.ExecuteAsync<AuthResponses>(AuthRequest);
            if(response.IsSuccessStatusCode && response.Data != null)
            {
                Properties.Settings.Default.SavedUserId = response.Data.Id;
                Properties.Settings.Default.SavedToken = response.Data.Token; 
                Properties.Settings.Default.Save();
                int registeredId = response.Data.Id;
                Contacts contacts = new Contacts(registeredId);
                contacts.Show();

                this.Close();
            }
            else
            {
                MessageBox.Show($"Сервер вернул ошибку: {response.ErrorMessage ?? response.Content}");
            }
            
        }
   
        private void click_Click(object sender, RoutedEventArgs e)
        {
            setData();
        }

        private void close_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
