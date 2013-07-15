# PhoneFinger

PhoneFinger transforms your Mac's mouse cursor into a realistic simulated human finger for testing and demoing iPhone applications and websites. It was released in 2008 as freeware and is now open-source.


### As Seen In Demos For
 <a href="http://www.kismetworldwide.com/WasteNot/">WasteNot</a><br/>
                <a href="http://www.ambrosiasw.com/games/akimobile/">Aki Mahjong</a><br/>
                <a href="http://www.youtube.com/watch?v=eB-bh55OrbQ">Comixology</a><br/>
                <a href="http://www.youtube.com/watch?v=eGCa2QAhC2A">WHERE</a><br/>
                <a href="http://www.youtube.com/watch?v=JjDTpWFl2kA">cocos2d</a><br/>
                <a href="http://cubecheater.efaller.com/">CubeCheater</a><br/>
                <a href="http://www.youtube.com/watch?v=3H8SeTyYXkY">Chris Pirillo</a><br/>
                <a href="http://www.youtube.com/watch?v=J2HdUuM_2Gk">Quantum Pool</a><br/>
                <a href="http://www.pentopuzzle.com/">Pentos</a>
 
 
### Hand Format
 <p>PhoneFinger uses its own <code>.hand</code> format to bundle hand graphics and related information. In addition to those that are built-in, PhoneFinger will use .hands from <code>~/Library/Application Support/PhoneFinger/Hands</code>.</p>

<p>A .hand file is actually a folder &mdash; you can right-click on it and choose &quot;Show Package Contents&quot; to look inside. Inside of a .hand package are the images the hand uses as well as an Info.plist file. So, to make a hand, just drop all the images and a customized Info.plist into a folder whose name ends with .hand, and drag it into the Hands folder. You'll have to restart PhoneFinger for the hand to show up.</p>

The Info.plist can contain the following values:
<dl class="plist-guide">
<dt><code>PFHandName</code></dt>
<dd>The readable name of your hand. Displayed in the hand model menu in the app.</dd>
<dt><code>PFHandAuthor</code><span class="opt">optional</span></dt>
<dd>The person or company responsible for the hand. Not displayed currently.</dd>
<dt><code>PFHandAuthorWebsite</code><span class="opt">optional</span></dt>
<dd>The URL or email associated with the above. Not displayed currently.</dd>
<dt><code>PFHandDescription</code><span class="opt">optional</span></dt>
<dd>A description of the hand. Not displayed currently.</dd>
<dt><code>PFHandVersion</code><span class="opt">optional</span></dt>
<dd>A version number for your hand to distinguish newer versions from older ones.</dd>
<dt><code>PFHandUpImage</code></dt>
<dd>The filename of the image that is displayed when no mouse button is pressed.</dd>
<dt><code>PFHandDownImage</code></dt>
<dd>The filename of the image that is displayed when at least one mouse button is pressed.</dd>
<dt><code>PFHandHotspotX</code></dt>
<dd>The location of the active part of the image (where a click will actually happen) expressed as a number of pixels from the left of the image </dd>
<dt><code>PFHandHotspotY</code></dt>
<dd>The location of the active part of the image (where a click will actually happen) expressed as a number of pixels from the top of the image </dd>
<dt><code>PFHandMinScaleFactor</code><span class="opt">optional</span></dt>
<dd>A number from 0 to 1.0 expressing the ratio to which your image will be scaled when the user selects the smallest size. You must also specify <code>PFHandMaxScaleFactor</code> if you specify this property.</dd>
<dt><code>PFHandMaxScaleFactor</code><span class="opt">optional</span></dt>
<dd>A number from 0 to 1.0 expressing the ratio to which your image will be scaled when the user selects the largest size.</dd>
</dl>
