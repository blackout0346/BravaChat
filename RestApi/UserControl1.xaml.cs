using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using System.Windows.Media;
using System.Windows.Threading;
using static RestApi.authification;
using MenuItem = System.Windows.Controls.MenuItem;
using MessageBox = System.Windows.MessageBox;
using UserControl = System.Windows.Controls.UserControl;
namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для UserControl1.xaml
    /// </summary>
    public partial class UserControl1 : UserControl
    {
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        private int Myid;
        private int ChatId;
        private int currentEditMessageId = 0;
        private int currentReplyMessageId = 0;

        private DispatcherTimer dispatcherTimer;
        public class MessageItem
        {

            [JsonPropertyName("Login")]
            public string login { get; set; }

            [JsonPropertyName("UserId")]
            public int SenderId { get; set; }

            [JsonPropertyName("message")]
            public string msg { get; set; }
            [JsonPropertyName("SendDate")]
            public string senddate { get; set; }
            //public HorizontalAlignment alignment { get; set; }
            public SolidColorBrush bubbleColor { get; set; }
            [JsonPropertyName("Id")]
            public int messageId { get; set; }
            [JsonPropertyName("EditedAt")]
            public string EditedAt { get; set; }
            public bool IsMyMessage {  get; set; }
        }
        
        public UserControl1(int myId, int chatId)
        {
            InitializeComponent();
            Myid = myId;
            ChatId = chatId;
            LoadMessages();
            dispatcherTimer = new DispatcherTimer();
            dispatcherTimer.Interval = TimeSpan.FromSeconds(2);
            dispatcherTimer.Tick += (s, e) => LoadMessages();
            dispatcherTimer.Start();
        }

        private async void LoadMessages()
        {
            try
            {
                RestRequest request = new RestRequest($"/users/messages/chat/{ChatId}", Method.Get);
                var response = await restClient.ExecuteAsync<List<MessageItem>>(request);
                if (response.IsSuccessStatusCode && response.Data != null)
                {
                    if (MessagesList.Items.Count != response.Data.Count)
                    {
                        MessagesList.Items.Clear();
                        foreach (var m in response.Data)
                        {
                            //if (m.SenderId == Myid)
                            //{
                            //    m.alignment = HorizontalAlignment.Right;
                            //    m.bubbleColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#DCF8C6"));
                            //}
                            //else
                            //{
                            //    m.alignment = HorizontalAlignment.Left;
                            //    m.bubbleColor = new SolidColorBrush(Colors.AliceBlue);
                            //}
                            m.IsMyMessage = (m.SenderId == Myid);

                            MessagesList.Items.Add(m);
                        }
                        if (MessagesList.Items.Count > 0)
                        {
                            MessagesList.ScrollIntoView(MessagesList.Items[MessagesList.Items.Count - 1]);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("error");
            }
        }
        
        private async void SetMessage()
        {
            var messages = new
            {
                userId = Myid,
                chatId = ChatId,
                message = InputMsg.Text,
                replyId = 0,
                forwardId = 0
                
            };
            RestRequest restRequest = new RestRequest("/users/messages", Method.Post);
            restRequest.AddJsonBody(messages);
            var response = await restClient.ExecuteAsync(restRequest);
            if (response.IsSuccessStatusCode)
            {
                InputMsg.Clear();
                currentReplyMessageId = 0;
                LoadMessages();
            }
            else
            {
                MessageBox.Show($"Ошибка: {response.StatusCode}\nОтвет сервера: {response.Content}");
            }
        }
        private MessageItem GetSelectedMessage(object sender)
        {
            if(sender is MenuItem menuItem)
            {
               if(menuItem.DataContext is MessageItem message)
                {
                    return message;
                }
            }
            return null;
        }
        private async void EditMessage(int messageId)
        {
            string newText = InputMsg.Text;
            string editTime = $"Изменено {DateTime.UtcNow:HH:mm}";
            var itemToUpdate = MessagesList.Items.Cast<MessageItem>().FirstOrDefault(m => m.messageId == messageId);
            if (itemToUpdate != null)
            {
                itemToUpdate.msg = newText;
                itemToUpdate.senddate = editTime;
                MessagesList.Items.Refresh();
            }

            RestRequest editRequest = new RestRequest($"/message/{messageId}/edit", Method.Put);
            editRequest.AddJsonBody(new { message = InputMsg.Text, sendDate = editTime});

            var response = await restClient.ExecuteAsync(editRequest);
     
            if (response.IsSuccessStatusCode)
            {
                HideActionPanel();
                
            }
            else
            {
                LoadMessages();
            }
        }
        private async void DeleteText(int messageId)
        {
         
            RestRequest deleteRequest = new RestRequest($"/message/{messageId}/delete", Method.Delete);
            string token = Properties.Settings.Default.SavedToken;
            deleteRequest.AddHeader("Authorization", "Bearer " + token);
            var response = await restClient.ExecuteAsync(deleteRequest);
            if (response.IsSuccessStatusCode)
            {
                LoadMessages();
            }
        }

        private void sendMessage_Click(object sender, RoutedEventArgs e)
        {
            if(string.IsNullOrWhiteSpace(InputMsg.Text) )
            {
                return;
            }
            if(currentEditMessageId !=0)
            {
                EditMessage(currentEditMessageId);
            }
            else
            {
                
                SetMessage();
                HideActionPanel();
            }
        
        }
        private void answer_Click(object sender, RoutedEventArgs e)
        {
            var msg = GetSelectedMessage(sender);
            if (msg == null)
            {
                return;
            }
            currentReplyMessageId = msg.messageId;
            currentEditMessageId = 0;
            InputMsg.Clear();
            ShowActionPanel($"Ответить от {msg.login}", msg.msg, MaterialDesignThemes.Wpf.PackIconKind.Reply);
            
        }

        private void edit_Click(object sender, RoutedEventArgs e)
        {
            var msg = GetSelectedMessage(sender);
            if (msg == null || msg.SenderId !=Myid)
            {
              
                return;
            }

            currentEditMessageId = msg.messageId;
            currentReplyMessageId = 0;
            InputMsg.Text = msg.msg;
            ShowActionPanel($"Редактировать", msg.msg, MaterialDesignThemes.Wpf.PackIconKind.Pencil);
            InputMsg.Focus();
        }

        private void forward_Click(object sender, RoutedEventArgs e)
        {
            var msg = GetSelectedMessage(sender);
            if (msg == null)
            {
                return;
            }
            MessageBox.Show($"Переслать...{msg.messageId}");
        }

        private void delete_Click(object sender, RoutedEventArgs e)
        {
            var msg = GetSelectedMessage(sender);
            if(msg == null)
            {
                return;
            }
            DeleteText(msg.messageId);
        }
        private void ShowActionPanel(string title, string text, MaterialDesignThemes.Wpf.PackIconKind icon)
        {
            ActionTitle.Text = title;
            ActionText.Text = text.Replace("\n", " ").Replace("\r", "");
            ActionIcon.Kind = icon;
            ActionPanel.Visibility = Visibility.Visible;
            ActionText.Visibility = Visibility.Visible;
            ActionIcon.Visibility = Visibility.Visible;
        }
        private void HideActionPanel()
        {
            ActionIcon.Visibility = Visibility.Collapsed;
            ActionPanel.Visibility = Visibility.Collapsed;
            ActionText.Visibility = Visibility.Collapsed;
            currentEditMessageId = 0;
            currentReplyMessageId= 0;
            InputMsg.Clear();
        }
        private void cancel_Click(object sender, RoutedEventArgs e)
        {
            HideActionPanel();
        }
    }
}