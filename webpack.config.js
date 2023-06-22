const path = require('path');
const UglifyJsPlugin = require('uglifyjs-webpack-plugin');
const webpack = require("webpack");

var define = new webpack.DefinePlugin({
    'process.env.NODE_ENV': JSON.stringify('production')
});
var ugly = new UglifyJsPlugin();

var _module =
{
	loaders: [
		{
			test: /\.js?/,
			include: path.resolve(__dirname, 'client-source/'),
			loader: 'babel-loader'
		}
	]
};

module.exports = [

	//Scripts.
	{
        plugins: [/*ugly*/define],
		entry: './client-source/l5rp-api.js',
		output: {
			filename: 'index.js',
			path: path.resolve(__dirname, 'bin/client_packages/')
		},

		module: _module
	},

];