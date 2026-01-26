using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
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

namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для authification.xaml
    /// </summary>
    public partial class authification : Window
    {
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        struct AuthificationData
        {
            public string Login {  get; set; }
            public string emails { get; set; }
            public string passwords { get; set; }
            public string numbers { get; set; }
        }
        private string Login = null;
        private string Email = null;
        private string Password = null;
        private string Number = null;
        
        public authification()
        {
            InitializeComponent();
    

        }

        private void setData()
        {
            Login = inputName.Text;
            Email = inputEmail.Text.Trim();
            Password = inputPassword.Text.Trim();
            Number = inputNumber.Text.Trim();
            if (Number.Length > 15)
            {
                MessageBox.Show("Некорректный номер");
          
            }
            if (!string.IsNullOrEmpty(Login) && !string.IsNullOrEmpty(Email) && !string.IsNullOrEmpty(Password))
            {
                AuthResponse(Login, Email, Password, Number);
                inputName.Text = " ";
                inputPassword.Text = " ";
                inputEmail.Text = " ";
                inputNumber.Text = " ";
                return;
            }
            else
            {
                MessageBox.Show($" {Login},{Email}, {Number}, {Password}Введите логин и пароль");
                MessageBox.Show("Введите все поля");
                inputName.Text = " ";
                inputPassword.Text = " ";
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
                numbers = number

            };
            RestRequest AuthRequest = new RestRequest("/auth", Method.Post);
            var jsonbody = JsonSerializer.Serialize(NewAuthUser);
            AuthRequest.AddJsonBody(jsonbody);
            RestResponse AuthResponse = await restClient.ExecuteAsync(AuthRequest);
            if(AuthResponse.IsSuccessful)
            {
                Contacts contacts = new Contacts();
                contacts.Show();
                var data = AuthResponse.Content;
                MessageBox.Show($"{data}");
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
