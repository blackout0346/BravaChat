using System.Linq;
using System.Net.Http;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;
using RestSharp;

namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        string inputsName;
        string inputsPassword;

        public MainWindow()
        {
            InitializeComponent();
       
        }
        struct myResponse
        {
            public string name { get; set; }
            public int price { get; set; }
        }
        struct Login
        {
            public string name { get; set; }
            public string password { get; set; }
        }
        private async void GetRequest()
        {
    
            RestRequest request = new RestRequest("/Users", Method.Get);
            request.AddHeader("username", "myname");


            RestResponse<myResponse> response = await restClient.ExecuteAsync<myResponse>(request);

            if (response.IsSuccessful)
            {
                var data = response.Data;

                var key = response.GetHeaderValue("secretkey");

                //output.Content = data.name + ' ' + data.price.ToString() + " " + key;
            }
        }
        private void setinput()
        {
            inputsName = inputName.Text.Trim();
            inputsPassword = inputPassword.Text.Trim();
            if (inputPassword.Text == " " || inputPassword.Text == " ")
            {
                MessageBox.Show($" {inputsName},{inputsPassword}Введите логин и пароль");
                return;
            }
            else
            {
                SETLOGIN(inputsName, inputsPassword);
                inputName.Text = " ";
                inputPassword.Text = " ";
                MessageBox.Show($" {inputsName},{inputsPassword}Что тут внутри?");
            }
        }
        private async void SETLOGIN(string inputsName, string inputsPassword)
        {

            var logins = new Login
            {
                name = inputsName,
                password = inputsPassword
            };
            RestRequest PostRequest = new RestRequest("/login", Method.Post);
            var jsonbody = JsonSerializer.Serialize(logins);
            PostRequest.AddJsonBody(jsonbody);
            RestResponse response = await restClient.ExecuteAsync(PostRequest);
            if (response.IsSuccessful)
            {
                MessageBox.Show($"{response}");   
            }
        }
        private async void click_Click(object sender, RoutedEventArgs e)
        {
            //GetRequest();
            setinput();
        }
       

    }
}
