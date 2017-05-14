var express = require('express');
var bodyParser = require('body-parser')
var app = express();
app.use(bodyParser.urlencoded({
    extended: true
}));
app.use(express.static('public'));


app.get('/', function(req, res) {
    res.sendFile(path.join(__dirname + 'index.html'));
});

app.use(bodyParser.json());

app.post('/submit', function(req, res) {
    // var data=req;
    console.log(req.body); 
    res.redirect('back');
});

app.listen(3000);
