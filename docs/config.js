const langs = [
    { title: 'English', path: '/home', matchPath: /^\/(home|ecosystem|support)/ },
    { title: '简体中文', path: '/en/', matchPath: /^\/en/ },
];

docute.init({
    landing: 'landing.html',
    title: 'Smart Farming',
    repo: 'iotcat/smart-farming',
    twitter: 'iotcat',
    'edit-link': 'https://github.com/IoTcat/smart-farming/blob/master/docs/',
    nav: {
        default: [
            {
                title: 'Home', path: '/home'
            },
            {
                title: 'Languages', type: 'dropdown', items: langs
            }
        ],
        'zh-Hans': [
            {
                title: '首页', path: '/en/'
            },
            {
                title: '选择语言', type: 'dropdown', items: langs
            }
        ],
    },
    plugins: [
        docsearch({
            apiKey: '46a8ba306e368972c1d75feb581fb430',
            indexName: 'wiot',
            tags: ['english', 'zh-Hans'],
            url: 'http://wiot.eee.dog'
        }),
        evanyou()
    ]
});

function loadScript (src, callback) {
    let s,
        r,
        t;
    r = false;
    s = document.createElement('script');
    s.type = 'text/javascript';
    s.src = src;
    s.onload = s.onreadystatechange = function () {
    // console.log( this.readyState ); //uncomment this line to see which ready states are called.
        if (!r && (!this.readyState || this.readyState == 'complete'))
        {
            r = true;
            callback();
        }
    };
    t = document.getElementsByTagName('script')[0];
    t.parentNode.insertBefore(s, t);
}
