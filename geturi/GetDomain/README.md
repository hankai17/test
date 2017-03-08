GetDomain
=========

Small command line application for getting domain from uri, it also checks the protocol such as:
<ol>
  <li>http</li>
  <li>https</li>
</ol>

<h2>How to Use:</h2>

The exe file is located in the bin directory, copy the full path of this folder to the PATH variable on your system:

<pre><code>
  PATH=C:\User\Username\Directory\bin;
</code></pre>

<b>Note:</b>Make sure to replace user, username and directory to your own names.

Now your setup and ready to use the app. Open up your command line and type the following:

<pre><code>
  c:\user\username\dir\bin> getdomain http://www.unity3d.com
</code></pre>

<b>Note:</b>Make sure to replace user, username and dir to your own names.

Press enter, the result should be the following:

<pre><code>
  c:\user\username\dir\bin> getdomain http://www.unity3d.com
  This protocol is standard
  
  Domain: www.unity3d.com
</code></pre>

The line "This protocol is standard" shows because http is the standard protocol. The next line "www.unity3d.com" is the domain name
in the uri.

Now do the same again but this time use the website:

<pre><code>
  https://github.com
</code></pre>

Once the changes were made, press enter. The result should be the following:

<pre><code>
  This protocol is secure
  
  Domain: github.com
</code></pre>

<h3>Endpoints parser:</h3>

Endpoints parser, retrieves the endpoints from the given uri (/endpoint/endpoint2/).

For this example we will use the instagram api endpoints, in the command line type the following:

<pre><code>
  c:\user\username\dir\bin> getdomain https://api.instagram.com/v1/media/popular
</code></pre>

<b>Note:</b>Make sure to replace user, username and directory to your own names.

Press enter, the result should be the following:

<pre><code>
  c:\user\username\dir\bin> getdomain https://api.instagram.com/v1/media/popular
  This protocol is secure
  
  Domain: api.instagram.com
  Endpoint: v1
  Endpoint: media
  Endpoint: popular
</code></pre>

The program now finds the endpoints in the uri.
