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
            [JsonPropertyName("Id")]
            public int Id { get; set; }
            [JsonPropertyName("message")]
            public string Message { get; set; }

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
            public int numbers { get; set; }

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
            passwords = inputPassword.Text.Trim();
            numbers = inputNumber.Text.Trim();
       
            if(!int.TryParse(numbers, out int input))
            {

          
            if (numbers.Length < 11)
            {
                MessageBox.Show("Некорректный номер");
                return;
            }
            if (!string.IsNullOrEmpty(Login) && !string.IsNullOrEmpty(emails) && !string.IsNullOrEmpty(passwords))
            {
                AuthResponse(Login, emails, passwords, input);
                inputName.Text = " ";
                inputPassword.Text = " ";
                inputEmail.Text = " ";
                inputNumber.Text = " ";
                return;
            }
            else
            {
               
                MessageBox.Show("Введите все поля");
                inputName.Text = " ";
                inputPassword.Text = " ";
                inputEmail.Text = " ";
                inputNumber.Text = " ";
                return;
            }

            }
        }

        private async void AuthResponse(string name, string email, string password, int number)
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
                int registeredId = response.Data.Id;
                Contacts contacts = new Contacts(registeredId);
                contacts.Show();
                MessageBox.Show($"Успешная регистрация! Ваш ID: {registeredId}");
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
