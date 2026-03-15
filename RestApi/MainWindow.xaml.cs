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
        private string inputsName = null;
        private string inputsPassword = null;
        
        public class UserResponse
        {
            [JsonPropertyName("Token")]
            public string Token { get; set; }
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
            public string NumberPhone {  get; set; }
        }
        public MainWindow()
        {
            InitializeComponent();
         
        }

        private void setinput()
        {
            inputsName = inputName.Text.Trim();
            inputsPassword = inputPassword.Password.Trim();
            if (!string.IsNullOrWhiteSpace(inputsName) && !string.IsNullOrWhiteSpace(inputsPassword))
            {
                SETLOGIN(inputsName, inputsPassword);
                inputsName = " ";
                inputsPassword = " ";
            }
            else
            {
                MessageBox.Show("Введите логин и пароль");
                inputsName = " ";
                inputsPassword = " ";
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
                var response = await restClient.ExecuteAsync(PostRequest);
                if (response.IsSuccessStatusCode)
                {
                try {
                    var userData = JsonSerializer.Deserialize<UserResponse>(response.Content);

                    Properties.Settings.Default.SavedUserId = userData.Id;
                    Properties.Settings.Default.SavedToken = userData.Token;
                    Properties.Settings.Default.Save();
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
        private void click_Click(object sender, RoutedEventArgs e)
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