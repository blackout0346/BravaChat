using System.Linq;
using System.Net.Http;
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

                output.Content = data.name + ' ' + data.price.ToString() + " " + key;
            }
        }
        private async void click_Click(object sender, RoutedEventArgs e)
        {
            //GetRequest();

            //if (inputName != null && inputPassword != null)
            //{
            //    SETLOGIN();
            //}
            //else
            //{
            //    return;
            //}
            string name = inputName.Text;
            string password = inputPassword.Text;
            Login login = new Login() { name=name, password = password};
            RestRequest PostRequest = new RestRequest("/login", Method.Post);
            PostRequest.AddJsonBody(login.ToString());
            RestResponse response = await restClient.ExecuteAsync(PostRequest);
            if (response.IsSuccessful)
            {
                output.Content = name + " " + password;
            }
        }
        private async void SETLOGIN()
        {
            string name = inputName.Text;
            string password = inputPassword.Text;
            RestRequest PostRequest = new RestRequest("/login", Method.Post);
            PostRequest.AddJsonBody(name, password);
            RestResponse response = await restClient.ExecuteAsync(PostRequest);
            if (response.IsSuccessful)
            {
                output.Content = name + " " + password;
            }
        }

    }
}
