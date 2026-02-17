using RestSharp;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Markup;

namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        string inputsName = null;
        string inputsPassword = null;

        class Logins
        {
            [JsonPropertyName("Login")]
            public string Login { get; set; }
            [JsonPropertyName("Password")]
            public string Password { get; set; }
            [JsonPropertyName("NumberPhone")]
            public string NumberPhone {  get; set; }
        }
        public MainWindow()
        {
            InitializeComponent();

        }

   
        private void setinput()
        {
            inputsName = inputName.Text.Trim();
            inputsPassword = inputPassword.Text.Trim();
            if (!string.IsNullOrEmpty(inputsName) && !string.IsNullOrEmpty(inputsPassword))
            {
                SETLOGIN(inputsName, inputsPassword);
                MessageBox.Show($" {inputsName},{inputsPassword}Что тут внутри?");
                inputName.Text = " ";
                inputPassword.Text = " ";
            }
            else
            {
                MessageBox.Show($" {inputsName},{inputsPassword}Введите логин и пароль");
                inputName.Text = " ";
                inputPassword.Text = " ";
                return;
            }

        }
        private async void SETLOGIN(string inputsName, string inputsPassword)
        {

            var logins = new Logins
            {
                Login = inputsName,
                Password = inputsPassword,
                NumberPhone = "0"
            };
            RestRequest PostRequest = new RestRequest("/login", Method.Post);
            
            PostRequest.AddJsonBody(logins);
            RestResponse response = await restClient.ExecuteAsync(PostRequest);
            if (response.IsSuccessStatusCode)
            {
                Contacts contacts = new Contacts();
                contacts.Show();
                 
            }
            else if(response.StatusCode == HttpStatusCode.Unauthorized)
            {
             
                MessageBox.Show($"Неверный логин или пароль {response.Content}");
            }
            else
            {

                MessageBox.Show($"Такого пользователя нет {response.Content}");
            }
        }
        private async void click_Click(object sender, RoutedEventArgs e)
        {
            //GetRequest();
            setinput();

        }

        private void link_Click(object sender, RoutedEventArgs e)
        {
            authification authification = new authification();
            authification.Show();

        }
    }
}