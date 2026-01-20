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
        string inputsName = null;
        string inputsPassword = null;
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
        public MainWindow()
        {
            InitializeComponent();
       
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
                Contacts contacts = new Contacts();
                contacts.Show();
                var data = response.StatusCode;
                MessageBox.Show($"{data}");   
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
