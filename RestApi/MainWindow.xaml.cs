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
        
        public class UserResponse
        {
            [JsonPropertyName("Id")]
            public int Id { get; set; }

            [JsonPropertyName("Login")]
            public string Login { get; set; }
        }
        class Logins
        {
            [JsonPropertyName("Login")]
            public string Login { get; set; }
            [JsonPropertyName("Password")]
            public string Password { get; set; }
            [JsonPropertyName("NumberPhone")]
            public int NumberPhone {  get; set; }
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
                NumberPhone = 0
            };
       
                RestRequest PostRequest = new RestRequest("/login", Method.Post);

                PostRequest.AddJsonBody(logins);
                var response = await restClient.ExecuteAsync(PostRequest);
                if (response.IsSuccessStatusCode)
                {
                try {
                    var userData = JsonSerializer.Deserialize<UserResponse>(response.Content);
                    Contacts contacts = new Contacts(userData.Id);
                    contacts.Show();
                    this.Close();
                }
                catch(JsonException)
                {
                    MessageBox.Show("Сервер прислал некорректный JSON, хотя статус 200"); 
                }

                }
                else if (response.StatusCode == HttpStatusCode.Unauthorized)
                {

                    MessageBox.Show($"Неверный логин или пароль {response.Content}");
                }
                else
                {

                MessageBox.Show($"Ошибка сервера: {response.Content}");
            }
            
        }
        private async void click_Click(object sender, RoutedEventArgs e)
        {

            setinput();

        }

        private void link_Click(object sender, RoutedEventArgs e)
        {
            authification authification = new authification();
            authification.Show();

        }
    }
}